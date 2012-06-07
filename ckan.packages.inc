<?php
/**
 * get nid from package id
 *
 */
function ckanpackage_nid_from_id($pid){
	$query = db_select('field_data_field_id', 'l');
	$query->fields('l', array('entity_id'))->condition('field_id_value', $pid, '=');
	$result = $query->execute();
	return key($result->fetchAllAssoc('entity_id'));
}

function ckan_taxonomy_get_term_by_value($v){
	$query = db_select('field_data_field_value', 'l');
	$query->fields('l', array('entity_id'))->condition('field_value_value', $v, '=');
	$result = $query->execute();
	return key($result->fetchAllAssoc('entity_id'));
}

/**
 * Create or edit the package node
 *
 */


function ckan_create_node($ckan_data,$licenses){
	$ckan_data=$ckan_data->result;
	$nid=ckanpackage_nid_from_id($ckan_data->id);
print_r($ckan_data);
	// if packaage exists then ->edit package ( creation of a new node revision)
	if($nid){
		$node=node_load($nid);
		if( $node->field_revision_id[LANGUAGE_NONE][0]['value'] != $ckan_data->revision_id	){
			$node->revision = TRUE;
			$node->log = "Updated programmatically from ckan @".date('Y-m-d h:i:s'); // Log message
		}
	}

	// if package dont exist create a new one
	else {
		$node = new stdClass();
		$node->uid = 1;
		$node->promote = 1;
		$node->sticky = 0;
		$node->type = "ckan_package";
		$node->language= LANGUAGE_NONE;
	}

	// common part for add andd edit  setting up the fields
	$node->title = $ckan_data->title;
	$node->field_id[LANGUAGE_NONE][0]['value'] = $ckan_data->id;
	$node->created =strtotime(str_replace("T"," ",$ckan_data-> metadata_created));
	$node->path = array('alias' => 'dataset/'.$ckan_data->name);
	if($ckan_data->notes){
		$body_text = $ckan_data->notes;
	$node->body[LANGUAGE_NONE][0]['value']   = $body_text;
	$node->body[LANGUAGE_NONE][0]['summary'] = text_summary($body_text);
	$node->body[LANGUAGE_NONE][0]['format']  = 'filtered_html';
	$node->field_notes[LANGUAGE_NONE][0]['value'] = $ckan_data->notes;
	$node->field_notes[LANGUAGE_NONE][0]['format'] ='filtered_html';
	}
	if($ckan_data->name)$node->field_name[LANGUAGE_NONE][0]['value'] = $ckan_data->name;
	if($ckan_data->url){$node->field_url[LANGUAGE_NONE][0]['url'] = $ckan_data->url;
	$node->field_url[LANGUAGE_NONE][0]['title'] = $ckan_data->name;
	}
	if($ckan_data->maintainer)$node->field_maintainer[LANGUAGE_NONE][0]['value'] = $ckan_data->maintainer;
	if($ckan_data->maintainer_email)$node->field_maintainer_email[LANGUAGE_NONE][0]['email'] = $ckan_data->maintainer_email;
	if($ckan_data->license_id)$node->field_license_id[LANGUAGE_NONE][0]['value'] = $ckan_data->license_id;
	if($ckan_data->revision_id)$node->field_revision_id[LANGUAGE_NONE][0]['value'] = $ckan_data->revision_id;
	if($licenses){$node->field_license[LANGUAGE_NONE][0]['title'] = $licenses[$ckan_data->license_id]['title'];
	$node->field_license[LANGUAGE_NONE][0]['url'] = $licenses[$ckan_data->license_id]['url'];
	}
  if(isset($ckan_data->groups))$node->field_groupe[LANGUAGE_NONE][0]['nid'] = ckangroup_nid_from_id($ckan_data->groups[0]->id);

 $arrextra=array();
	if(isset($ckan_data->extras))foreach($ckan_data->extras as $ext)$arrextra[$ext->key]=$ext->value;

  if($arrextra){
	if(isset($arrextra['dct:contributor']))$node->field_contributor[LANGUAGE_NONE][0]['value'] = json_decode($arrextra['dct:contributor']);
	if(isset($arrextra['dct:creator']))$node->field_creator[LANGUAGE_NONE][0]['value'] = json_decode($arrextra['dct:creator']);
	if(isset($arrextra['dct:publisher']))$node->field_publisher[LANGUAGE_NONE][0]['nid'] = ckangroup_nid_from_id(trim($arrextra['dct:publisher']));
	if(isset($arrextra['dct:temporal']))$node->field_temporal[LANGUAGE_NONE][0]['value'] = json_decode($arrextra['dct:temporal']);
	if(isset($arrextra['maj'])){
		$node->field_maj[LANGUAGE_NONE][0]['value'] = json_decode($arrextra['maj']);
		$node->field_maj[LANGUAGE_NONE][0]['format'] ='filtered_html';
	}

	if(isset($arrextra['dc:source']))$node->field_source[LANGUAGE_NONE][0]['value'] = json_decode($arrextra['dc:source']);
	if(isset($arrextra['dcterms:references']))$node->field_reference[LANGUAGE_NONE][0]['value'] = json_decode($arrextra['dcterms:references']);
  }



	// managing tags :adding tags as taonomies terms
	if(isset($ckan_data->tags)){
		$tags =$ckan_data->tags;
	  $arrtids=array();
	  $arrtags=array();
		if($tags)foreach($tags as $t)$arrtags[]=ckantag_tid_from_id($t->id);
		if(isset($node->field_tagsckan[LANGUAGE_NONE]))foreach($node->field_tagsckan[LANGUAGE_NONE] as $k=>$v){
		$arrtids[]=$v['tid'];if(!in_array($v['tid'],$arrtags))unset($node->field_tagsckan[LANGUAGE_NONE][$k]['tid']);}
	  if($arrtags)foreach($arrtags as $k=> $v)if(!in_array($v,$arrtids))$node->field_tagsckan[LANGUAGE_NONE][$k]['tid']=$v;

	}



	// managing frequencies
	if(isset($ckan_data->frequencies[0])){
		$fre =$ckan_data->frequencies[0];
		$tidf=key(taxonomy_term_load_multiple(array(), array('name' => $fre, 'vid' => variable_get('ckan_frequencevocab'))));
		if($fre<>''&& $tidf<>'')$node->field_frequence[LANGUAGE_NONE][0]['tid']=$tidf;
	}

	// managing qualite
	if(isset($ckan_data->dataQuality[0])){
		$ql =$ckan_data->dataQuality[0];
			$tidq=key(taxonomy_term_load_multiple(array(), array('name' => $ql, 'vid' => variable_get('ckan_qualitevocab'))));
		if($ql<>''&& $tidq<>'')$node->field_qualite[LANGUAGE_NONE][0]['tid']=$tidq;
	}
	// managing granualite
	if(isset($ckan_data->granularity[0])){
		$gr =$ckan_data->granularity[0];
			$tidg=key(taxonomy_term_load_multiple(array(), array('name' => $gr, 'vid' => variable_get('ckan_granularitevocab'))));
		if($gr<>''&& $tidg<>'')$node->field_granularite[LANGUAGE_NONE][0]['tid']=$tidg;
	}
	// managing concept
	if(isset($ckan_data->themeTaxonomy[0])){
		$cp =$ckan_data->themeTaxonomy[0];
		if($cp<>'' && ckan_taxonomy_get_term_by_value($cp)<>'')$node->field_concept[LANGUAGE_NONE][0]['tid']=ckan_taxonomy_get_term_by_value($cp);
	}
 // managing categories
	if(isset($ckan_data->theme_available[0])){
		$ca =$ckan_data->theme_available[0];
		if($ca<>''&& ckan_taxonomy_get_term_by_value($ca)<>'')$node->field_categorie[LANGUAGE_NONE][0]['tid']=ckan_taxonomy_get_term_by_value($ca);
	}




	//adding or editingextra fields in content type ckan_extrafields/////////////////////////
		$extras=array();$arrex=array();
	$arrex=array("dct:creator","dct:publisher","dct:temporal","maj","dct:contributor","dcterms:references","dc:source");
	if($arrextra){
	$extras=$arrextra;
	if(isset($node->field_extras[LANGUAGE_NONE]))foreach($node->field_extras[LANGUAGE_NONE] as $k=>$ext)if(!in_array($ext,$extras))unset($node->field_extras[LANGUAGE_NONE][$k]['nid']);
	foreach ($extras as $name => $value) {
		if(!in_array($name,$arrex)){
		/// check if the field exists
		$nidextra=ckan_checkextra($name,$value);
		if($nidextra && $nidextra<>'' && (isset($node->field_extras) && !in_array($nidextra,$node->field_extras[LANGUAGE_NONE])))$node->field_extras[LANGUAGE_NONE][]['nid']=$nidextra;
		else{
			/// create a field node
			$nidextra=ckan_createnodeextra($name,$value);
			if($nidextra && $nidextra<>'')$node->field_extras[LANGUAGE_NONE][]['nid']=$nidextra;
			}
	}
	}
	}
	/////////////////////////// fin fields extras///////////////////////////////////////////

	//adding or editing ressources fields in content type ckan_resoources/////////////////////////
	if(isset($ckan_data->resources)){
		$ressources=array();
	$ressources=(array) $ckan_data->resources;
	if(isset($node->field_ressources[LANGUAGE_NONE]))foreach($node->field_ressources[LANGUAGE_NONE] as $k=>$ext)if(!in_array($ext,$ressources))unset($node->field_ressources[LANGUAGE_NONE][$k]['nid']);
	foreach ($ressources as $res) {
		/// check if the field exists
		$nidressource=ckan_checkressource($res->id);
		if($nidressource && $nidressource<>''){
			if( !in_array($nidressource,$node->field_ressources[LANGUAGE_NONE]))$node->field_ressources[LANGUAGE_NONE][]['nid']=$nidressource;
			ckan_createnoderessource($res,$nidressource);
		}
		else{
			/// create a field node
			$nidressource=ckan_createnoderessource($res);
			$node->field_ressources[LANGUAGE_NONE][]['nid']=$nidressource;
		}
	}
	}
	/////////////////////////// fin fields ressources///////////////////////////////////////////
//print_r($node);
	$node=node_submit($node);
	node_save($node);
	if($nid){drupal_set_message(t("Package @title edited",array("@title"=>$ckan_data->title)));
	echo t("Package @title edited",array("@title"=>$ckan_data->title));
	}
	else {drupal_set_message(t("Package @title added",array("@title"=>$ckan_data->title)));
	echo t("Package @title added",array("@title"=>$ckan_data->title));
	}
}


/**
 * get the package
 *
 */

function ckan_new($ckan_name,$datelastimport){
	$licenses=ckan_licenses();

	$ckan = ckan_ckan();
	try {
	$data['id']=$ckan_name;
			$ckan_data = $ckan->getPackage($data);
		//print_r($ckan_data);
			} catch (CkanException $e){
		drupal_set_message($e->getMessage(),  "error node package creation");
		drupal_set_title( "error node package creation");
		return  "error node package creation";
	}
	$datecreated=date('Y-m-d',strtotime(str_replace("T"," ",$ckan_data->result->metadata_created)));
	$datemodified=date('Y-m-d',strtotime(str_replace("T"," ",$ckan_data->result->metadata_modified)));
	$datecreated=strtotime($datecreated);
	$datemodified=strtotime($datemodified);
	//if($datelastimport && ($datecreated>=$datelastimport || $datemodified>=$datelastimport) ){
		echo "<br/>Package Added or edited Since last import :".$ckan_data->result->name;
		$node = ckan_create_node($ckan_data,$licenses);
	//}
	return 1;
}

/**
 * get the list of packages
 *
 */
function ckan_list(){

	$datelastimport=variable_get("ckan_lastimport",strtotime('today'));
	variable_set("ckan_lastimport",strtotime('today'));
	$ckan = ckan_ckan();
	try {
		$ckan_list = $ckan->getPackageList();

		if($ckan_list)foreach($ckan_list as $k=>$ck){
			//print_r($ckan_list);
			ckan_new($ck,$datelastimport);

		}
		cache_clear_all();
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list package");
		drupal_set_title("error list package");
		return "error list package";
	}

}


/**
 * create or edit a node ckan_ressources
 *
 */
function ckan_createnoderessource($res,$nidres=''){
	$arrbasicfields=array("resource_group_id","cache_last_updated","package_id","webstore_last_updated","id","size","last_modified","hash","description","format","tracking_summary",
"mimetype_inner","mimetype","cache_url","name","created","url","webstore_url","position","resource_type","owner","cache_url_updated");
	if($nidres<>''){$nd=node_load($nidres);$nd->revision=1;$nd->log="updatedddddd";}
	else{
		$nd = new stdClass(); // We create a new node object
		$nd->type = "ckan_ressources"; // Or any other content type you want
		$nd->field_ressourceid[LANGUAGE_NONE][0]['value'] = $res->id;

	}
	if(isset($res->created))$nd->created =strtotime(str_replace("T"," ",$res->created));
	if(isset($res->last_modified))$nd->changed=strtotime(str_replace("T"," ",$res->last_modified));
	$nd->title = $res->name;
	$nd->uid = 1;
	$nd->language = LANGUAGE_NONE;
	if(isset($res->description)){
	$nd->body[LANGUAGE_NONE][0]['value']   = $res->description;
	$nd->body[LANGUAGE_NONE][0]['summary'] = text_summary($res->description);
	$nd->body[LANGUAGE_NONE][0]['format']  = 'filtered_html';
	}
	if(isset( $res->size))$nd->field_size[LANGUAGE_NONE][0]['value'] = $res->size;
	if(isset( $res->url))$nd->field_ressourceurl[LANGUAGE_NONE][0]['value'] = $res->url;
	if(isset( $res->webstore_url))$nd->field_webstore_url[LANGUAGE_NONE][0]['value'] = $res->webstore_url;
	if(isset( $res->resource_type))$nd->field_resource_type[LANGUAGE_NONE][0]['value'] = $res->resource_type;
	if(isset( $res->position))$nd->field_position[LANGUAGE_NONE][0]['value'] = $res->position;
	if(isset( $res->mimetype))$nd->field_mimetype[LANGUAGE_NONE][0]['value'] = $res->mimetype;
	if(isset( $res->mimetype_inner))$nd->field_mimetype_inner[LANGUAGE_NONE][0]['value'] = $res->mimetype_inner;

	/////////adding fields extra////////////////////////////////
	if($res)foreach($res as $k =>$v)if(!in_array($k,$arrbasicfields))$extras[$k]=$v;
	if(isset($nd->field_ressourceextras[LANGUAGE_NONE]))foreach($nd->field_ressourceextras[LANGUAGE_NONE] as $k=>$ext)if(!in_array($ext,$extras))unset($nd->field_ressourceextras[LANGUAGE_NONE][$k]['nid']);
	if(isset($extras))foreach ($extras as $name => $value) {
		/// check if the field exists
		$nidextra=ckan_checkextra($name,$value);
		if($nidextra && $nidextra<>'' && is_array($nd->field_ressourceextras[LANGUAGE_NONE]))if(!in_array($nidextra,$nd->field_ressourceextras[LANGUAGE_NONE]))$nd->field_ressourceextras[LANGUAGE_NONE][]['nid']=$nidextra;
		else{
			/// create a field node
			$nidextra=ckan_createnodeextra($name,$value);
			$nd->field_extras[LANGUAGE_NONE][]['nid']=$nidextra;
		}
	}
	//////////////////////////////
	$ndres = node_submit($nd); // Prepare node for a submit
	node_save($ndres);
	$nidres=$ndres->nid;
	return $nidres;
}

/**
 * check if a ckan_ressource node exista already
 *
 */

function ckan_checkressource($id){
	$query = db_select('field_data_field_ressourceid', 'r');
	$query->fields('r', array('entity_id'))->condition('r.field_ressourceid_value', $id, '=');
	$result = $query->execute();
	$nidres=key($result->fetchAllAssoc('entity_id'));
	return $nidres;
}
