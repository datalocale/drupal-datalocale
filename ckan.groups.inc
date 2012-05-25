<?php


/**
 * get ckan group list  and managing parent groups and users groups
 *
 */

function ckan_groups(){

$ckan = ckan_ckan();
	 try {
	 $groups = $ckan->getGroupList();
 if($groups)foreach($groups as $k=>$g)
	 $groupe= group_new($g);

	 global $arrgroups;
 	 global $arrusers;

	 foreach($arrgroups as $idp=>$gr){
	 	$nidparent=ckangroup_nid_from_id($idp);
	 	foreach($gr as $idfils){
	 		$nidfils=ckangroup_nid_from_id($idfils);
	 		$n=node_load($nidfils);
	 		unset($n->field_groupes[LANGUAGE_NONE]);
	 		$n->field_groupes[LANGUAGE_NONE][]['nid']=$nidparent;
	 		node_save($n);
	 	}

	 }
	 	 foreach($arrusers as $idu=>$grs){
	 	$uiduser=ckan_checkuser($idu);
	 	$usr=user_load($uiduser);
	 	$edit = (array) $usr;
	 	unset($edit['field_ugroupes']['und']);

	 	foreach($grs as $gr){

	 		$nidgr=ckangroup_nid_from_id($gr);

$edit['field_ugroupes']['und'][]['nid'] = $nidgr;
user_save($usr, $edit);
	 	}

	 }



	 }

	 catch (CkanException $e){
	 drupal_set_message($e->getMessage(), "error list groups");
	 drupal_set_title("error list groups");
	 return "error list groups";
	 }

}

/**
 * get ckan group details
 *
 */


function group_new($group_name){
	$ckan = ckan_ckan();
	try {
		$group_data = $ckan->getGroup($group_name);
		//print_r($group_data);
	} catch (CkanException $e){
		drupal_set_message($e->getMessage(),  "error getGroup creation");
		drupal_set_title( "error getGroup creation");
		return  "error getGroup creation";
	}
	return ckan_create_group($group_data);

}
/**
 * get drupal nid group from ckan group id
 *
 */

function ckangroup_nid_from_id($pid){
	$query = db_select('field_data_field_grid', 'l');
	$query->fields('l', array('entity_id'))->condition('field_grid_value', $pid, '=');
	$result = $query->execute();
	return key($result->fetchAllAssoc('entity_id'));
}

/**
 * Add a group node
 *
 */

function ckan_create_group($group_data){
	$nid=ckangroup_nid_from_id($group_data->id);
  //print_r($ckan_data);
	// if packaage exists then ->edit package ( creation of a new node revision)
	if($nid){
		$node=node_load($nid);
		if( $node->field_grrevision_id[LANGUAGE_NONE][0]['value'] != $group_data->revision_id	){
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
		$node->type = "groupe";
		$node->language= LANGUAGE_NONE;
	}

	// common part for add andd edit  setting up the fields
	$node->title = $group_data->title;
	$node->field_grid[LANGUAGE_NONE][0]['value'] = $group_data->id;
	$node->created =strtotime(str_replace("T"," ",$group_data-> created));
	$node->path = array('alias' => 'groupe/'.$group_data->name);
	$body_text = $group_data->description;
	$node->body[LANGUAGE_NONE][0]['value']   = $body_text;
	$node->body[LANGUAGE_NONE][0]['summary'] = text_summary($body_text);
	$node->body[LANGUAGE_NONE][0]['format']  = 'filtered_html';
	$node->field_grname[LANGUAGE_NONE][0]['value'] = $group_data->name;
	$node->field_grdisplay_name[LANGUAGE_NONE][0]['value'] = $group_data->display_name;
	$node->field_image_url[LANGUAGE_NONE][0]['value'] = $group_data->image_url;
	$node->field_grrevision_id[LANGUAGE_NONE][0]['value'] = $group_data->revision_id;
	$node->field_type[LANGUAGE_NONE][0]['value'] = $group_data->type;



	//adding or editingextra fields in content type ckan_extrafields/////////////////////////
	$extras=array();
	$extras=(array) $group_data->extras;
if(isset($node->field_grextras[LANGUAGE_NONE]))foreach($node->field_grextras[LANGUAGE_NONE] as $k=>$ext)if(!in_array($ext,$extras))unset($node->field_grextras[LANGUAGE_NONE][$k]['nid']);
	foreach ($extras as $name => $value) {
		/// check if the field exists
		$nidextra=ckan_checkextra($name,$value);
		if($nidextra && $nidextra<>'' && (isset($node->field_grextras) && !in_array($nidextra,$node->field_grextras[LANGUAGE_NONE])))$node->field_grextras[LANGUAGE_NONE][]['nid']=$nidextra;
		else{
			/// create a field node
			$nidextra=ckan_createnodeextra($name,$value);
			if($nidextra && $nidextra<>'')$node->field_grextras[LANGUAGE_NONE][]['nid']=$nidextra;
		}
	}

	/////////////////////////// fin fields extras///////////////////////////////////////////

	///////////////////////////groups////////////////////////////////////////////////
		 global $arrgroups;
	$groups=$group_data->groups;
	if($groups)foreach($groups as $gr)$arrgroups[$group_data->id][]=$gr->id;

	/////////////////// group users/////////////////
	 global $arrusers;
	$users=$group_data->users;
	if($users)foreach($users as $ur)$arrusers[$ur->id][]=$group_data->id;



	$node=node_submit($node);
	node_save($node);
	if($nid)drupal_set_message(t("Group @title edited",array("@title"=>$group_data->title)));
	else drupal_set_message(t("Group @title added",array("@title"=>$group_data->title)));
}



///////////////////////////////////////////FIN CKan groups///////////////////////////////////////
