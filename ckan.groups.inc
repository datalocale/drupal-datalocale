<?php


/**
 * get ckan group list  and managing parent groups and users groups
 *
 */

function ckan_groups(){

$ckan = ckan_ckan();
	 try {
	 $groups = $ckan->getGroupList();
	 //print_r($groups);
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
	 	 foreach($arrusers as $nameu=>$grs){
	 	$usr=user_load_by_name($nameu);
	 	$edit = (array) $usr;
	 	unset($edit['field_ugroupes']['und']);

	 	foreach($grs as $gr){

	 		$nidgr=ckangroup_nid_from_id($gr);

$edit['field_ugroupes']['und'][]['nid'] = $nidgr;
//print_r($edit);
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
		$data['id']=$group_name;
	$data['all_fields']=1;
		$group_data = $ckan->getGroup($data);
		print_r($group_data);
	} catch (CkanException $e){
		drupal_set_message($e->getMessage(),  "error getGroup creation");
		drupal_set_title( "error getGroup creation");
		return  "error getGroup creation";
	}
	return ckan_create_group($group_data->result);

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
 // print_r($group_data);
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
	//$node->created =strtotime(str_replace("T"," ",$group_data-> created));
	//$node->path = array('alias' => 'groupe/'.$group_data->name);
	$body_text = $group_data->description;
	$node->body[LANGUAGE_NONE][0]['value']   = $body_text;
	$node->body[LANGUAGE_NONE][0]['summary'] = text_summary($body_text);
	$node->body[LANGUAGE_NONE][0]['format']  = 'filtered_html';
	if($group_data->type=="service")$node->path = array('alias' => 'producteur/'.$group_data->name);
	elseif($group_data->type=="organization")$node->path = array('alias' => 'diffuseur/'.$group_data->name);
	if(isset($group_data->revision_id))$node->field_grrevision_id[LANGUAGE_NONE][0]['value'] = $group_data->revision_id;
	if(isset($group_data->name))$node->field_grname[LANGUAGE_NONE][0]['value'] = $group_data->name;
	if(isset($group_data->type))$node->field_type[LANGUAGE_NONE][0]['value'] = $group_data->type;
	if(isset($group_data->{"postal-code"}))$node->field_gcodepostal[LANGUAGE_NONE][0]['value'] = json_decode($group_data->{"postal-code"});
	if(isset($group_data->{"country-name"}))$node->field_gcountry[LANGUAGE_NONE][0]['value'] = json_decode($group_data->{"country-name"});
	if(isset($group_data->locality))$node->field_glocality[LANGUAGE_NONE][0]['value'] = json_decode($group_data->locality);
	if(isset($group_data->url))$node->field_url[LANGUAGE_NONE][0]['value'] = json_decode($group_data->url);
	if(isset($group_data->{"foaf:name"}))$node->field_gname[LANGUAGE_NONE][0]['value'] =json_decode($group_data->{"foaf:name"});
	if(isset($group_data->phone))$node->field_gphone[LANGUAGE_NONE][0]['value'] = json_decode($group_data->phone);
	if(isset($group_data->mail))$node->field_gemail[LANGUAGE_NONE][0]['email'] = json_decode($group_data->mail);
	if(isset($group_data->{"street-address"}))$node->field_streetaddress[LANGUAGE_NONE][0]['value'] = json_decode($group_data->{"street-address"});
	

	
	
	// managing image url 
	if($group_data->image_url<>''){
	$imgpath=$group_data->image_url;
  $directory = file_build_uri('groups');
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
    // If our directory doesn't exist and can't be created, use the default.
    $directory = NULL;
  }
  $file = system_retrieve_file($imgpath, $directory, TRUE,FILE_EXISTS_REPLACE); 
if($file->fid<>'' && $file)$node->field_gimage[LANGUAGE_NONE][0] = (array) $file;

	}	



	//adding or editingextra fields in content type ckan_extrafields/////////////////////////
	if($group_data->additional_extras){
	$arrextra=array();
	foreach($group_data->additional_extras as $ext)$arrextra[$ext->key]=$ext->value;
if(isset($node->field_grextras[LANGUAGE_NONE]))foreach($node->field_grextras[LANGUAGE_NONE] as $k=>$ext)if(!in_array($ext,$arrextra))unset($node->field_grextras[LANGUAGE_NONE][$k]['nid']);
	foreach ($arrextra as $name => $value) {
		/// check if the field exists
		$nidextra=ckan_checkextra($name,$value);
		if($nidextra && $nidextra<>'' && (isset($node->field_grextras) && !in_array($nidextra,$node->field_grextras[LANGUAGE_NONE])))$node->field_grextras[LANGUAGE_NONE][]['nid']=$nidextra;
		else{
			/// create a field node
			$nidextra=ckan_createnodeextra($name,$value,1);
			if($nidextra && $nidextra<>'')$node->field_grextras[LANGUAGE_NONE][]['nid']=$nidextra;
		}
	}
	}

	/////////////////////////// fin fields extras///////////////////////////////////////////

	///////////////////////////groups////////////////////////////////////////////////
		 global $arrgroups;
	$groups=$group_data->children;
	if($groups)foreach($groups as $gr)$arrgroups[$group_data->id][]=$gr->id;

	/////////////////// group users/////////////////
	 global $arrusers;
	$users=$group_data->users;
	if($users)foreach($users as $ur)$arrusers[$ur->name][]=$group_data->id;



	$node=node_submit($node);
	node_save($node);
	if($nid){drupal_set_message(t("Group @title edited",array("@title"=>$group_data->title)));
	echo t("Group @title edited",array("@title"=>$group_data->title))."<br/>";
	}
	else{
		drupal_set_message(t("Group @title added",array("@title"=>$group_data->title)));
		echo t("Group @title added",array("@title"=>$group_data->title))."<br/>";
	}
}



///////////////////////////////////////////FIN CKan groups///////////////////////////////////////
