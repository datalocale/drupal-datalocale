<?php
/**
 * get all tags lists
 *
 */

function ckan_tags(){
	ckan_licence();
	ckan_tagslist();
	ckan_tagsq();
	ckan_tagsg();
	ckan_tagsf();
	ckan_tagst();
	ckan_tagscat();
	ckan_tagsgeo();
}
/**
 * check if the tag already exists
 *
 */

function ckan_checktag($id){
	$query = db_select('field_data_field_tagid', 'c');
	$query->join('taxonomy_term_data ', 't', 'c.entity_id = t.tid');
	$query->fields('c', array('entity_id'))->condition('c.field_tagid_value', $id, '=');
	$result = $query->execute();
	$tid=key($result->fetchAllAssoc('entity_id'));
	return $tid;
	
}


/**
 * get tag tid from ckan id
 *
 */
function ckantag_tid_from_id($pid){
	$query = db_select('field_data_field_tagid', 'l');
	$query->fields('l', array('entity_id'))->condition('field_tagid_value', $pid, '=');
	$result = $query->execute();
	return key($result->fetchAllAssoc('entity_id'));
}
/**
 * get free tags list
 *
 */
function ckan_tagslist(){
	$ckanvocab=variable_get('ckan_tagsvocab');
	$ckan = ckan_ckan();
	try {
		$data['all_fields']=1;
		$tags = $ckan->gettags($data);
		if($tags->result)foreach($tags->result as $k=>$tg){
			$tid=ckan_checktag($tg->id);
			if($tid){
				$term=taxonomy_term_load($tid);
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab;
				drupal_set_message(t("Tag @title edited",array("@title"=>$tg->display_name)));
				echo t("Tag @title edited",array("@title"=>$tg->display_name))."<br/>";

			}else{
				$term = new stdClass();
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab; 
				$term->field_tagid[LANGUAGE_NONE][0]['value']=$tg->id;
				drupal_set_message(t("Tag @title added",array("@title"=>$tg->display_name)));
				echo t("Tag @title added",array("@title"=>$tg->display_name))."<br/>";

			}
			taxonomy_term_save($term);
		}
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list tags");
		drupal_set_title("error list tags");
		return "error list tags";
	}
}
/**
 * get quality tags list
 *
 */
function ckan_tagsq(){
	$ckanvocab=variable_get('ckan_qualitevocab');
	$ckan = ckan_ckan();
	try {
		$data['all_fields']=1;
		$data['vocabulary_id']="dcat:dataQuality";
		$tags = $ckan->gettags($data);
		if($tags->result)foreach($tags->result as $k=>$tg){
			$tid=ckan_checktag($tg->id);
			if($tid){
				$term=taxonomy_term_load($tid);
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab;
				drupal_set_message(t("Tag @title edited",array("@title"=>$tg->display_name)));
				echo t("Tag @title edited",array("@title"=>$tg->display_name))."<br/>";

			}else{
				$term = new stdClass();
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab; 
				$term->field_tagid[LANGUAGE_NONE][0]['value']=$tg->id;
				drupal_set_message(t("Tag @title added",array("@title"=>$tg->display_name)));
				echo t("Tag @title added",array("@title"=>$tg->display_name))."<br/>";

			}
			taxonomy_term_save($term);
		}
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list tags");
		drupal_set_title("error list tags");
		return "error list tags";
	}

}
/**
 * get granularity tags list
 *
 */
function ckan_tagsg(){
	$ckanvocab=variable_get('ckan_granularitevocab');
	$ckan = ckan_ckan();
	try {
		$data['all_fields']=1;
		$data['vocabulary_id']="dcat:granularity";
		$tags = $ckan->gettags($data);
		if($tags->result)foreach($tags->result as $k=>$tg){
			$tid=ckan_checktag($tg->id);
			if($tid){
				$term=taxonomy_term_load($tid);
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab;
				drupal_set_message(t("Tag @title edited",array("@title"=>$tg->display_name)));
				echo t("Tag @title edited",array("@title"=>$tg->display_name))."<br/>";

			}else{
				$term = new stdClass();
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab; 
				$term->field_tagid[LANGUAGE_NONE][0]['value']=$tg->id;
				drupal_set_message(t("Tag @title added",array("@title"=>$tg->display_name)));
				echo t("Tag @title added",array("@title"=>$tg->display_name))."<br/>";

			}
			taxonomy_term_save($term);
		}
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list tags");
		drupal_set_title("error list tags");
		return "error list tags";
	}

}
/**
 * get frequence tags list
 *
 */

function ckan_tagsf(){
	$ckanvocab=variable_get('ckan_frequencevocab');
	$ckan = ckan_ckan();
	try {
		$data['all_fields']=1;
		$data['vocabulary_id']="dct:accrualPeriodicity";
		$tags = $ckan->gettags($data);
		if($tags->result)foreach($tags->result as $k=>$tg){
			$tid=ckan_checktag($tg->id);
			if($tid){
				$term=taxonomy_term_load($tid);
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab;
				drupal_set_message(t("Tag @title edited",array("@title"=>$tg->display_name)));
				echo t("Tag @title edited",array("@title"=>$tg->display_name))."<br/>";

			}else{
				$term = new stdClass();
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab; 
				$term->field_tagid[LANGUAGE_NONE][0]['value']=$tg->id;
				drupal_set_message(t("Tag @title added",array("@title"=>$tg->display_name)));
				echo t("Tag @title added",array("@title"=>$tg->display_name))."<br/>";

			}
			taxonomy_term_save($term);
		}
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list tags");
		drupal_set_title("error list tags");
		return "error list tags";
	}

}
/**
 * get concept tags list
 *
 */
function ckan_tagst(){
	$ckanvocab=variable_get('ckan_conceptvocab');
	$ckan = ckan_ckan();
	try {
		$data['all_fields']=1;
		$data['vocabulary_id']="dcat:themeTaxonomy";
		$tags = $ckan->gettagsdatalocale($data);
		//print_r($tags);
		if($tags->result)foreach($tags->result as $k=>$tg){
			$tid=ckan_checktag($tg->id);
			if($tid){
				$term=taxonomy_term_load($tid);
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab;
				drupal_set_message(t("Tag @title edited",array("@title"=>$tg->display_name)));
				echo t("Tag @title edited",array("@title"=>$tg->display_name))."<br/>";

			}else{
				$term = new stdClass();
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab; 
				$term->field_tagid[LANGUAGE_NONE][0]['value']=$tg->id;
				$term->field_value[LANGUAGE_NONE][0]['value']=$tg->name;
				drupal_set_message(t("Tag @title added",array("@title"=>$tg->display_name)));
				echo t("Tag @title added",array("@title"=>$tg->display_name))."<br/>";

			}
			taxonomy_term_save($term);
		}
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list tags");
		drupal_set_title("error list tags");
		return "error list tags";
	}


}


function ckan_tagscat(){
	$ckanvocab=variable_get('ckan_categorievocab');
	$ckan = ckan_ckan();
	try {
		$data['id']="dcat:themeTaxonomy";
		$data['recursive']=1;
		$data['all_fields']=1;
		$tags = $ckan->tagscat($data);
		//print_r($tags);break;

	 if($tags->result)foreach($tags->result as $tgs){
	 	foreach($tgs as $tg){

	 		//$term = taxonomy_get_term_by_name($tg[1]);

	 		$term =taxonomy_term_load_multiple(array(), array('name' => $tg[1], 'vid' => $ckanvocab));
	 		$tid=key($term);
	 		if($tid){
	 			$term=taxonomy_term_load($tid);
	 			$term->name = $tg[1];
	 			$term->vid = $ckanvocab;
	 			$term->field_value[LANGUAGE_NONE][0]['value']=$tg[0];
	 			drupal_set_message(t("Tag @title edited",array("@title"=>$tg[1])));
	 			echo t("Tag @title edited",array("@title"=>$tg[1]))."<br/>";

	 		}else{
	 			$term = new stdClass();
	 			$term->name = $tg[1];
	 			$term->vid = $ckanvocab; 
	 			// l 'id est eliminé dans cette categorie
	 			//$term->field_tagid[LANGUAGE_NONE][0]['value']=$tg->id;
	 			$term->field_value[LANGUAGE_NONE][0]['value']=$tg[0];
	 			drupal_set_message(t("Tag @title added",array("@title"=>$tg[1])));
	 			echo t("Tag @title added",array("@title"=>$tg[1]))."<br/>";

	 		}
	 		taxonomy_term_save($term);
	 	}
	 }
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error cat tags");
		drupal_set_title("error cat tags");
		return "error cat tags";
	}


}

/**
 * get geographical tags list
 *
 */
function ckan_tagsgeo(){
	$ckanvocab=variable_get('ckan_geographicvocab');
	$ckan = ckan_ckan();
	try {
		$data['all_fields']=1;
		$data['vocabulary_id']="geographic_granularity";
		$tags = $ckan->gettagsdatalocale($data);
		if($tags->result)foreach($tags->result as $k=>$tg){
			$tid=ckan_checktag($tg->id);
			if($tid){
				$term=taxonomy_term_load($tid);
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab;
				drupal_set_message(t("Tag @title edited",array("@title"=>$tg->display_name)));
				echo t("Tag @title edited",array("@title"=>$tg->display_name))."<br/>";

			}else{
				$term = new stdClass();
				$term->name = $tg->display_name;
				$term->vid = $ckanvocab; 
				$term->field_tagid[LANGUAGE_NONE][0]['value']=$tg->id;
				$term->field_value[LANGUAGE_NONE][0]['value']=$tg->name;
				drupal_set_message(t("Tag @title added",array("@title"=>$tg->display_name)));
				echo t("Tag @title added",array("@title"=>$tg->display_name))."<br/>";

			}
			taxonomy_term_save($term);
		}
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list tags");
		drupal_set_title("error list tags");
		return "error list tags";
	}


}
/**
 * get licence list
 *
 */
function ckan_licence(){
	$ckanvocab=variable_get('ckan_licencevocab');
	$ckan = ckan_ckan();
	try {
		$licenses = $ckan->getLicenseList();
		// print_r($licenses);
		if($licenses)foreach($licenses as $k=>$g){
			//if($i<=10)
			$arrlic[$g->id]['title']=$g->title;
			$arrlic[$g->id]['url']=$g->url;
			// echo ckan_create_license($g);$i++;

		}
		if($arrlic)foreach($arrlic as $k=>$tg){
			$tid=ckan_checktag($k);
			if($tid){
				$term=taxonomy_term_load($tid);
				$term->name = $tg['title'];
				$term->vid = $ckanvocab;
				drupal_set_message(t("Tag @title edited",array("@title"=> $tg['title'])));
				echo t("Tag @title edited",array("@title"=>$tg['title']))."<br/>";

			}else{
				$term = new stdClass();
				$term->name =  $tg['title'];
				$term->vid = $ckanvocab; 
				$term->field_tagid[LANGUAGE_NONE][0]['value']=$k;
				$term->field_value[LANGUAGE_NONE][0]['value']=$tg['url'];
				drupal_set_message(t("Licence @title added",array("@title"=> $tg['title'])));
				echo t("Licence @title added",array("@title"=> $tg['title']))."<br/>";

			}
			taxonomy_term_save($term);
		}
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list Licence");
		drupal_set_title("error list Licence");
		return "error list Licence";
	}

}