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
 * get taxonomy term bye url
 *
 */
function ckan_taxonomy_get_term_by_value($v){
	$query = db_select('field_data_field_value', 'l');
	$query->fields('l', array('entity_id'))->condition('field_value_value', $v, '=');
	$result = $query->execute();
	return key($result->fetchAllAssoc('entity_id'));
}
/**
 * get taxonomy term by ckan id
 *
 */
function ckan_taxonomy_get_termid_by_ckanid($v){
	$query = db_select('field_data_field_tagid', 'l');
	$query->fields('l', array('entity_id'))->condition('field_tagid_value', $v, '=');
	$result = $query->execute();
	return key($result->fetchAllAssoc('entity_id'));
}

/**
 * Create or edit the package node
 *
 */


function ckan_create_node($ckan_data,$licenses){

	$ckan_data=$ckan_data->result;
	print_r($ckan_data);
	$nid=ckanpackage_nid_from_id($ckan_data->id);
	//print_r($ckan_data);
	$arrextra=array();
	if(isset($ckan_data->extras))foreach($ckan_data->extras as $ext)$arrextra[$ext->key]=$ext->value;
	$arrextraad=array();
	if(isset($ckan_data->additional_extras))foreach($ckan_data->additional_extras as $ext)$arrextraad[$ext->key]=$ext->value;
	//print_r($arrextra);
	//echo "test:".$ckan_data->ckan_author."enddd <br/>";
	if(isset($ckan_data->ckan_author)&& json_decode($ckan_data->ckan_author)<>'')
	$uid=ckan_checkuser(json_decode($ckan_data->ckan_author));
	if(!isset($uid) ||$uid=='')$uid=1;
	//echo $uid."uiddddddddd";
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

		$node->promote = 0;
		$node->sticky = 0;
		$node->type = "ckan_package";
		$node->language= LANGUAGE_NONE;
		$node->created =strtotime(str_replace("T"," ",$ckan_data-> metadata_created));
	}

	// common part for add andd edit  setting up the fields
	$node->uid = $uid;
    $node->comment = 2;
	if($ckan_data->state=="active")$state=1; else $state=0;
	$node->status =$state;
	$node->title = $ckan_data->title;
	$node->field_id[LANGUAGE_NONE][0]['value'] = $ckan_data->id;
	echo $ckan_data-> metadata_created;// break;
	
	$node->path ['alias'] = 'dataset/'.$ckan_data->name;
	if($ckan_data->notes){
		$body_text = $ckan_data->notes;
		$node->body[LANGUAGE_NONE][0]['value']   = $body_text;
		$node->body[LANGUAGE_NONE][0]['summary'] = text_summary($body_text);
		$node->body[LANGUAGE_NONE][0]['format']  = 'filtered_html';
	}
	$arrdate=array(
    'value' => date('Y-m-d H:i:s', strtotime(str_replace("T"," ",$ckan_data-> metadata_created))),
      'timezone' => 'UTC',
      'timezone_db' => 'UTC',
  );
	$node->field_createdckan[LANGUAGE_NONE][0]['value']=$arrdate;
	if($ckan_data->name)$node->field_name[LANGUAGE_NONE][0]['value'] = $ckan_data->name;
	if($ckan_data->url){$node->field_url[LANGUAGE_NONE][0]['url'] = $ckan_data->url;
	$node->field_url[LANGUAGE_NONE][0]['title'] = $ckan_data->name;
	}
	if(isset($ckan_data->maintainer))$node->field_maintainer[LANGUAGE_NONE][0]['value'] = $ckan_data->maintainer;
	if(isset($ckan_data->maintainer_email))$node->field_maintainer_email[LANGUAGE_NONE][0]['email'] = $ckan_data->maintainer_email;
	if(isset($ckan_data->author))$node->field_ckanauthor[LANGUAGE_NONE][0]['value'] = $ckan_data->author;
	if(isset($ckan_data->author_email))$node->field_ckanauthoremail[LANGUAGE_NONE][0]['value'] = $ckan_data->author_email;

	if(isset($ckan_data->{'spatial-text'}))$node->field_spatialnom[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->{'spatial-text'});
	if(isset($ckan_data->{'spatial-uri'}))$node->field_spatialurl[LANGUAGE_NONE][0]['value'] =json_decode ($ckan_data->{'spatial-uri'});
	if(isset($ckan_data->{"dct:accrualPeriodicity-other"}))$node->field_autref[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->{"dct:accrualPeriodicity-other"});
	if(isset($ckan_data->{'geographic_granularity-other'}))$node->field_autreg[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->{'geographic_granularity-other'});


	if(isset($ckan_data->{'temporal_coverage-from'})&& json_decode($ckan_data->{'temporal_coverage-from'})<>'')$node->field_couverturetemporelle[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->{'temporal_coverage-from'});
	if(isset($ckan_data->{'temporal_coverage-to'})&& json_decode($ckan_data->{'temporal_coverage-to'})<>'')$node->field_couverturetemporelle[LANGUAGE_NONE][0]['value2'] = json_decode($ckan_data->{'temporal_coverage-to'});
	if(isset($ckan_data->license_id))$node->field_license_id[LANGUAGE_NONE][0]['value'] = $ckan_data->license_id;
	if(isset($ckan_data->revision_id))$node->field_revision_id[LANGUAGE_NONE][0]['value'] = $ckan_data->revision_id;
	/*if($licenses){$node->field_license[LANGUAGE_NONE][0]['title'] = $licenses[$ckan_data->license_id]['title'];
	 $node->field_license[LANGUAGE_NONE][0]['url'] = $licenses[$ckan_data->license_id]['url'];
	 }*/
	//if(isset($ckan_data->groups))$node->field_groupe[LANGUAGE_NONE][0]['nid'] = ckangroup_nid_from_id($ckan_data->groups[0]->id);


	/// managing spatial elements//////////////////////
	if(isset($ckan_data->spatial)){
		$geophp = geophp_load();
		$sp=json_decode($ckan_data->spatial,true);
		$arrspatial=json_decode($sp, true);
		$wkt2=$arrspatial['type'].' (('.$arrspatial['coordinates'][0][0][0].' '.$arrspatial['coordinates'][0][0][1].', '.$arrspatial['coordinates'][0][1][0].' '.$arrspatial['coordinates'][0][1][1].', '.$arrspatial['coordinates'][0][2][0].' '.$arrspatial['coordinates'][0][2][1].', '.$arrspatial['coordinates'][0][3][0].' '.$arrspatial['coordinates'][0][3][1].', '.$arrspatial['coordinates'][0][4][0].' '.$arrspatial['coordinates'][0][4][1].'))';
		// echo $wkt2;
		//$wkt_bounds_format = 'POLYGON((left bottom,right bottom,right top,left top,left bottom))';
		//$wkt = strtr($wkt_bounds_format, array('top' => 45.5731, 'bottom' => 44.194, 'right' => 0.315056, 'left' =>  -1.26156));
		$geometry = geoPHP::load($wkt2, 'wkt');
		$arrextent = geofield_get_values_from_geometry($geometry);
		//print_r($arrextent);

		if(isset($ckan_data->spatial))$node->field_spatialextent[LANGUAGE_NONE][0] = $arrextent;
	}
	////////////////////////////////////


	if(isset($ckan_data->{"dct:contributor"}))$node->field_contributor[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->{"dct:contributor"});
	if(isset($ckan_data->{'dct:creator'}))$node->field_creator[LANGUAGE_NONE][0]['nid'] = ckangroup_nid_from_id(json_decode($ckan_data->{'dct:creator'}));
	if(isset($ckan_data->{'dct:publisher'}))$node->field_publisher[LANGUAGE_NONE][0]['nid'] = ckangroup_nid_from_id(json_decode($ckan_data->{'dct:publisher'}));
	//if(isset($ckan_data->{'dct:publisher'}))$node->field_publisher[LANGUAGE_NONE][0]['nid'] = ckangroup_nid_from_id(json_decode($arrextra['published_by']));
	//if(isset($ckan_data->{'dct:temporal'}))$node->field_temporal[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->{'dct:temporal'});
	if(isset($ckan_data->{'maj'})){
		$node->field_maj[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->{'maj'});
		$node->field_maj[LANGUAGE_NONE][0]['format'] ='filtered_html';
	}

	if(isset($ckan_data->{'dc:source'}))$node->field_source[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->{'dc:source'});
	if(isset($ckan_data->{'dcterms:references'}))$node->field_reference[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->{'dcterms:references'});
	if(isset($ckan_data->{"dcat:granularity"}))$node->field_granularity[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->{'dcat:granularity'});
	// managing scores////

	if(isset($ckan_data->openness_score_html)){
		$node->field_scorehtml[LANGUAGE_NONE][0]['format']  = 'fullhtml';
		$node->field_scorehtml[LANGUAGE_NONE][0]['value'] = $ckan_data->openness_score_html;
	}
	if(isset($ckan_data->openness_score))$node->field_score[LANGUAGE_NONE][0]['value'] = json_decode($ckan_data->openness_score);
	if(isset($ckan_data->openness_score))$node->field_scoref[LANGUAGE_NONE][0]['count'] = json_decode($ckan_data->openness_score);

	// managing tags :adding tags as taonomies terms
	if(isset($ckan_data->tags)){
		$tags =$ckan_data->tags;
		$arrtids=array();
		$arrtags=array();
		if($tags)foreach($tags as $t)$arrtags[]=ckantag_tid_from_id($t->id);
		if(isset($node->field_tagsckan[LANGUAGE_NONE]))foreach($node->field_tagsckan[LANGUAGE_NONE] as $k=>$v){
			$arrtids[]=$v['tid'];if(!in_array($v['tid'],$arrtags))unset($node->field_tagsckan[LANGUAGE_NONE][$k]['tid']);}
	  if($arrtags)foreach($arrtags as $k=> $v)if(!in_array($v,$arrtids)&& $v<>'')$node->field_tagsckan[LANGUAGE_NONE][$k]['tid']=$v;

	}

	// managing licence
	if(isset($ckan_data->license_id)){
		$ll=$ckan_data->license_id;
		if($ll<>'' && ckan_taxonomy_get_termid_by_ckanid($ll)<>'')$node->field_ckanlicence[LANGUAGE_NONE][0]['tid']=ckan_taxonomy_get_termid_by_ckanid($ll);
	}


	// managing frequencies
	if(isset($ckan_data->{"dct:accrualPeriodicity"})){
		$fre =$ckan_data->{"dct:accrualPeriodicity"};
		$tidf=key(taxonomy_term_load_multiple(array(), array('name' => $fre, 'vid' => variable_get('ckan_frequencevocab'))));
		if($fre<>''&& $tidf<>'')$node->field_frequence[LANGUAGE_NONE][0]['tid']=$tidf;
	}

	// managing qualite
	if(isset($ckan_data->dataQuality[0])){
		$ql =$ckan_data->dataQuality[0];
		$tidq=key(taxonomy_term_load_multiple(array(), array('name' => $ql, 'vid' => variable_get('ckan_qualitevocab'))));
		if($ql<>''&& $tidq<>'')$node->field_qualite[LANGUAGE_NONE][0]['tid']=$tidq;
	}

	// managing concept
	if(isset($ckan_data->themeTaxonomy)){
		$cpar=(array)$ckan_data->themeTaxonomy;
		$cp =key($cpar);
		if($cp<>'' && ckan_taxonomy_get_term_by_value($cp)<>'')$node->field_concept[LANGUAGE_NONE][0]['tid']=ckan_taxonomy_get_term_by_value($cp);
	}
	// managing categories
	if(isset($ckan_data->theme_available)){
		$caar=(array)$ckan_data->theme_available;
		$ca =key($caar); //echo "taxo".ckan_taxonomy_get_term_by_value($ca)."fintest<br/>";
		if($ca<>''&& ckan_taxonomy_get_term_by_value($ca)<>''){
			//$node->field_categorie[LANGUAGE_NONE][0]['tid']=ckan_taxonomy_get_term_by_value($ca);
		$node->field_concept[LANGUAGE_NONE][1]['tid']=ckan_taxonomy_get_term_by_value($ca);}
	}

	// managing geographical granularity
	if(isset($ckan_data->geographic_granularity)){
		$gg=$ckan_data->geographic_granularity;
		$tidgg=key(taxonomy_term_load_multiple(array(), array('name' => $gg, 'vid' => variable_get('ckan_geographicvocab'))));
		if($gg<>''&& $tidgg<>'')$node->field_geogranularity[LANGUAGE_NONE][0]['tid']=$tidgg;
	}





	// managing image url
	if(isset($ckan_data->image_url) && json_decode($ckan_data->image_url)<>''){
		$imgpath=json_decode($ckan_data->image_url);


		$directory = file_build_uri('packages');
		if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
			// If our directory doesn't exist and can't be created, use the default.
			$directory = NULL;
		}
		$file = system_retrieve_file($imgpath, $directory, TRUE,FILE_EXISTS_REPLACE);
		//print_r($file);

		if($file->fid<>'')$node->field_image[LANGUAGE_NONE][0] = (array) $file;

	}




	//adding or editingextra fields in content type ckan_extrafields/////////////////////////
	$extras=array();
	//$arrex=array();
	//$arrex=array("dct:creator","dct:publisher","dct:temporal","maj","dct:contributor","dcterms:references","dc:source");
	//if($arrextra){
	$extras=$arrextraad;
	if(isset($node->field_extras[LANGUAGE_NONE]))foreach($node->field_extras[LANGUAGE_NONE] as $k=>$ext)if(!in_array($ext,$extras))unset($node->field_extras[LANGUAGE_NONE][$k]);
	foreach ($extras as $name => $value) {
		$nidextra='';
		/// check if the field exists
		$nidextra=ckan_checkextra($name,$value);
		if($nidextra && $nidextra<>'' && (isset($node->field_extras) && !in_array($nidextra,$node->field_extras[LANGUAGE_NONE])))$node->field_extras[LANGUAGE_NONE][]['nid']=$nidextra;
		else{
			/// create a field node
			$nidextra=ckan_createnodeextra($name,$value,$uid);
			if($nidextra && $nidextra<>'')$node->field_extras[LANGUAGE_NONE][]['nid']=$nidextra;
		}

	}
	//}
	/////////////////////////// fin fields extras///////////////////////////////////////////


	//adding or editing ressources fields in content type ckan_resoources/////////////////////////
	if(isset($ckan_data->resources)){
		$ressources=array();
		$ressources=(array) $ckan_data->resources;
		if(isset($node->field_ressources[LANGUAGE_NONE]))foreach($node->field_ressources[LANGUAGE_NONE] as $k=>$ext)if(!in_array($ext,$ressources))unset($node->field_ressources[LANGUAGE_NONE][$k]);
		foreach ($ressources as $res) {
			/// check if the field exists
			$nidressource=ckan_checkressource($res->id);
			if($nidressource && $nidressource<>''){
				if( !in_array($nidressource,$node->field_ressources[LANGUAGE_NONE])&& $nidressource && $nidressource<>'')$node->field_ressources[LANGUAGE_NONE][]['nid']=$nidressource;
			}
			else{
				/// create a field node
				$nidressource=ckan_createnoderessource($res,'',$uid);
				if($nidressource && $nidressource<>'')$node->field_ressources[LANGUAGE_NONE][]['nid']=$nidressource;
			}
		}
	}
	
	/////////////////////////// fin fields ressources///////////////////////////////////////////
	
	$node=node_submit($node);
	node_save($node);
	print_r($node);break;
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
		$data['all_fields']=1;
		$ckan_data = $ckan->getPackage($data);
		print_r($ckan_data);
	} catch (CkanException $e){
		drupal_set_message($e->getMessage(),  "error node package creation");
		drupal_set_title( "error node package creation");
		return  "error node package creation";
	}
	if($ckan_data->success==1){
		$datecreated=date('Y-m-d',strtotime(str_replace("T"," ",$ckan_data->result->metadata_created)));
		$datemodified=date('Y-m-d',strtotime(str_replace("T"," ",$ckan_data->result->metadata_modified)));
		$datecreated=strtotime($datecreated);
		$datemodified=strtotime($datemodified);
		//if($datelastimport && ($datecreated>=$datelastimport || $datemodified>=$datelastimport) ){
		if(!isset($ckan_data->error)){
			echo "<br/>Package Added or edited Since last import :".$ckan_data->result->name;
			$node = ckan_create_node($ckan_data,$licenses);
		}
		//}
	}
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
		$data['all_fields']=1;
		$ckan_list = $ckan->getPackageList($data);
		$ckan_list=$ckan_list->result;
		if($ckan_list)foreach($ckan_list as $k=>$ck){
				
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
function ckan_createnoderessource($res,$nidres='',$uid=''){
	//print_r($res);
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
	$nd->uid = $uid;
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

	// managing image url
	if(isset($res->url) && $res->url<>''){
		$filepath=$res->url;


		$directory = file_build_uri('ressources');
		if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
			// If our directory doesn't exist and can't be created, use the default.
			$directory = NULL;
		}
		$file = system_retrieve_file($filepath, $directory, TRUE,FILE_EXISTS_REPLACE);
		$file->grid=1;
        $file->graph = 0;
        $file->map = 1;
        $file->timeline = 1;
        $file->transform =0;

		if($file->fid<>'')$nd->field_ressourcefile[LANGUAGE_NONE][0] = (array) $file;

	}








	/////////adding fields extra////////////////////////////////
	if($res)foreach($res as $k =>$v)if(!in_array($k,$arrbasicfields))$extras[$k]=$v;
	if(isset($nd->field_ressourceextras[LANGUAGE_NONE]))foreach($nd->field_ressourceextras[LANGUAGE_NONE] as $k=>$ext)if(!in_array($ext,$extras))unset($nd->field_ressourceextras[LANGUAGE_NONE][$k]['nid']);
	if(isset($extras))foreach ($extras as $name => $value) {
		/// check if the field exists
		$nidextra=ckan_checkextra($name,$value);
		if($nidextra && $nidextra<>'' && is_array($nd->field_ressourceextras[LANGUAGE_NONE]))if(!in_array($nidextra,$nd->field_ressourceextras[LANGUAGE_NONE]))$nd->field_ressourceextras[LANGUAGE_NONE][]['nid']=$nidextra;
		else{
			/// create a field node
			$nidextra=ckan_createnodeextra($name,$value,$uid);
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
