<?php
/**
 * check if the tag already exists
 *
 */

function ckan_checktag($id){
	$query = db_select('field_data_field_tagid', 'c');
	$query->fields('c', array('entity_id'))->condition('c.field_tagid_value', $id, '=');
	$result = $query->execute();
	$tid=key($result->fetchAllAssoc('entity_id'));
	return $tid;
}
/**
 * get tags list
 *
 */
function ckan_tags(){
	$ckanvocab=variable_get('ckan_tagsvocab');
	$ckan = ckan_ckan();
	try {
		$tags = $ckan->gettags();

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
				$term->vid = $ckanvocab; // ‘1’ is a vocabulary id you wish this term to assign to
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
 * get tag tid from ckan id
 *
 */
function ckantag_tid_from_id($pid){
	$query = db_select('field_data_field_tagid', 'l');
	$query->fields('l', array('entity_id'))->condition('field_tagid_value', $pid, '=');
	$result = $query->execute();
	return key($result->fetchAllAssoc('entity_id'));
}
