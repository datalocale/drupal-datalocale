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

/**
 * Create or edit the package node
 *
 */


function ckan_create_node($ckan_data,$licenses){
	$nid=ckanpackage_nid_from_id($ckan_data->id);
  //print_r($ckan_data);
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
	$body_text = $ckan_data->notes_rendered;
	$node->body[LANGUAGE_NONE][0]['value']   = $body_text;
	$node->body[LANGUAGE_NONE][0]['summary'] = text_summary($body_text);
	$node->body[LANGUAGE_NONE][0]['format']  = 'filtered_html';
	$node->field_name[LANGUAGE_NONE][0]['value'] = $ckan_data->name;
	$node->field_name[LANGUAGE_NONE][0]['format'] =$ckan_data->name;
	$node->field_url[LANGUAGE_NONE][0]['url'] = $ckan_data->url;
	$node->field_url[LANGUAGE_NONE][0]['title'] = $ckan_data->name;
	$node->field_notes[LANGUAGE_NONE][0]['value'] = $ckan_data->notes;
	$node->field_notes[LANGUAGE_NONE][0]['format'] ='filtered_html';
	$node->field_maintainer[LANGUAGE_NONE][0]['value'] = $ckan_data->maintainer;
	$node->field_maintainer_email[LANGUAGE_NONE][0]['email'] = $ckan_data->maintainer_email;
	$node->field_license_id[LANGUAGE_NONE][0]['value'] = $ckan_data->license_id;
	$node->field_revision_id[LANGUAGE_NONE][0]['value'] = $ckan_data->revision_id;
	$node->field_license[LANGUAGE_NONE][0]['title'] = $licenses[$ckan_data->license_id]['title'];
	$node->field_license[LANGUAGE_NONE][0]['url'] = $licenses[$ckan_data->license_id]['url'];



	// managing tags :adding tags as taonomies terms
	$tags =$ckan_data->tags;
	$ckanvocab=variable_get('ckan_tagsvocab');
	if(isset($node->field_tagsckan[LANGUAGE_NONE]))foreach($node->field_tagsckan[LANGUAGE_NONE] as $k=>$v)if(!in_array($v,$tags))unset($node->field_tagsckan[LANGUAGE_NONE][$k]['tid']);
	if($tags && $ckanvocab)foreach ($tags as $key => $tag) {
		if ($term = taxonomy_get_term_by_name($tag)) {
			$terms_array = array_keys($term);
			if($terms_array['0']<>'')$node->field_tagsckan[LANGUAGE_NONE][$key]['tid'] = $terms_array['0'];
		} else {
			$term = new stdClass();
			$term->name = $tag;
			$term->vid = $ckanvocab;
			if (!empty($term->name)) {
				$test = taxonomy_term_save($term);
				$term = taxonomy_get_term_by_name($tag);
				if($term){$tid = key($term);
				if($tid<>'')$node->field_tagsckan[LANGUAGE_NONE][$key]['tid'] = $tid;
				}
			}
		}
	}

	//adding or editingextra fields in content type ckan_extrafields/////////////////////////
	$extras=array();
	$extras=(array) $ckan_data->extras;
	if(isset($node->field_extras[LANGUAGE_NONE]))foreach($node->field_extras[LANGUAGE_NONE] as $k=>$ext)if(!in_array($ext,$extras))unset($node->field_extras[LANGUAGE_NONE][$k]['nid']);
	foreach ($extras as $name => $value) {
		/// check if the field exists
		$nidextra=ckan_checkextra($name,$value);
		if($nidextra && $nidextra<>'' && (isset($node->field_extras) && !in_array($nidextra,$node->field_extras[LANGUAGE_NONE])))$node->field_extras[LANGUAGE_NONE][]['nid']=$nidextra;
		else{
			/// create a field node
			$nidextra=ckan_createnodeextra($name,$value);
			$node->field_extras[LANGUAGE_NONE][]['nid']=$nidextra;
		}
	}

	/////////////////////////// fin fields extras///////////////////////////////////////////

	//adding or editing ressources fields in content type ckan_resoources/////////////////////////
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

	/////////////////////////// fin fields ressources///////////////////////////////////////////

	$node=node_submit($node);
	node_save($node);
}

/**
 * get the package
 *
 */

function ckan_new($ckan_name,$datelastimport){
	$licenses=ckan_licenses();

	$ckan = ckan_ckan();
	try {
		$ckan_data = $ckan->getPackage($ckan_name);
		//print_r($ckan_data);
	} catch (CkanException $e){
		drupal_set_message($e->getMessage(),  "error node package creation");
		drupal_set_title( "error node package creation");
		return  "error node package creation";
	}
	$datecreated=date('Y-m-d',strtotime(str_replace("T"," ",$ckan_data->metadata_created)));
	$datemodified=date('Y-m-d',strtotime(str_replace("T"," ",$ckan_data->metadata_modified)));
	$datecreated=strtotime($datecreated);
	$datemodified=strtotime($datemodified);
	//if($datelastimport && ($datecreated>=$datelastimport || $datemodified>=$datelastimport) ){
		echo "<br/>Package Added or edited Since last import :".$ckan_data->name;
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
 * create a node extrafield
 *
 */
function ckan_createnodeextra($name,$value){
	$nd = new stdClass(); // We create a new node object
	$nd->type = "ckan_extrafields"; // Or any other content type you want
	$nd->title = "Extra field  ".$name;
	$node->uid = 1;
	$nd->language = LANGUAGE_NONE;
	//$nd->created =strtotime(str_replace("T"," ",$ckan_data-> metadata_created));
	//$nd->changed=strtotime(str_replace("T"," ",$ckan_data-> metadata_last_modified));
	$nd->field_labelchamp[LANGUAGE_NONE][0]['value'] = $name;
	$nd->field_valuechamp[LANGUAGE_NONE][0]['value'] = $value;
	$ndextra = node_submit($nd); // Prepare node for a submit
	node_save($ndextra);
	$nidextra=$ndextra->nid;
	return $nidextra;
}
/**
 * check if a node extrafield exists already
 *
 */
function ckan_checkextra($name,$value){
	$query = db_select('field_data_field_labelchamp', 'l');
	$query->join('field_data_field_valuechamp', 'v', 'l.entity_id = v.entity_id'); // inner_join file_usage table against file_managed
	$query->fields('l', array('entity_id'))->condition('l.field_labelchamp_value', $name, '=')->condition("v.field_valuechamp_value",$value,'=');
	$result = $query->execute();
	$nidextra=key($result->fetchAllAssoc('entity_id'));
	return $nidextra;
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
	$nd->created =strtotime(str_replace("T"," ",$res->created));
	$nd->changed=strtotime(str_replace("T"," ",$res->last_modified));
	$nd->title = $res->name;
	$nd->uid = 1;
	$nd->language = LANGUAGE_NONE;
	$nd->body[LANGUAGE_NONE][0]['value']   = $res->description;
	$nd->body[LANGUAGE_NONE][0]['summary'] = text_summary($res->description);
	$nd->body[LANGUAGE_NONE][0]['format']  = 'filtered_html';
	$nd->field_size[LANGUAGE_NONE][0]['value'] = $res->size;
	$nd->field_ressourceurl[LANGUAGE_NONE][0]['value'] = $res->url;
	$nd->field_webstore_url[LANGUAGE_NONE][0]['value'] = $res->webstore_url;
	$nd->field_resource_type[LANGUAGE_NONE][0]['value'] = $res->resource_type;
	$nd->field_position[LANGUAGE_NONE][0]['value'] = $res->position;
	$nd->field_mimetype[LANGUAGE_NONE][0]['value'] = $res->mimetype;
	$nd->field_mimetype_inner[LANGUAGE_NONE][0]['value'] = $res->mimetype_inner;

	/////////adding fields extra////////////////////////////////
	foreach($res as $k =>$v)if(!in_array($k,$arrbasicfields))$extras[$k]=$v;
	if(isset($nd->field_ressourceextras[LANGUAGE_NONE]))foreach($nd->field_ressourceextras[LANGUAGE_NONE] as $k=>$ext)if(!in_array($ext,$extras))unset($nd->field_ressourceextras[LANGUAGE_NONE][$k]['nid']);
	if(isset($extras))foreach ($extras as $name => $value) {
		/// check if the field exists
		$nidextra=ckan_checkextra($name,$value);
		if($nidextra && $nidextra<>'' && !in_array($nidextra,$nd->field_ressourceextras[LANGUAGE_NONE]))$nd->field_ressourceextras[LANGUAGE_NONE][]['nid']=$nidextra;
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