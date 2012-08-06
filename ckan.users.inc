<?php

/**
 * users
 *
 */
function ckan_checkuser($id){
	$query = db_select('field_data_field_idutilisateur', 'u');
	$query->fields('u', array('entity_id'))->condition('field_idutilisateur_value', $id, '=');
	$result = $query->execute();
	return key($result->fetchAllAssoc('entity_id'));
}
/**
 * get ckan users list
 *
 */

function ckan_users(){

	$ckan = ckan_ckan();
	try {
		$data['id']='';
	$users = $ckan->getckanusers($data);
  getusers($users);
		cache_clear_all();
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list users");
		drupal_set_title("error list users");
		return "error list users";
	}

}
/**
 * get ckan role
 *
 */

function ckan_userrole($username){

	$ckan = ckan_ckan();
	try {
		$data['id']=$username;
	$role = $ckan->getckanuserrole($data);
	return $role;

		cache_clear_all();
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list users");
		drupal_set_title("error list users");
		return "error list users";
	}

}

/**
 * add ckan users in drupal
 *
 */

function getusers($users){
require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
	if($users->result)foreach($users->result as $u){
;
$uid=ckan_checkuser($u->id);

if(!$uid){
 $account = new stdClass();
$account->is_new = TRUE;
$account->status = TRUE;
$account->name = $u->name;
if($u->openid=='')$account->pass = user_hash_password($u->name);
$account->mail = $u->email;
$account->init = $u->email;
//$account->roles[5] = 'some role';
$account->field_display_name[LANGUAGE_NONE][0]['value'] = $u->display_name;
$account->field_idutilisateur[LANGUAGE_NONE][0]['value'] = $u->id;
$account->field_fullname[LANGUAGE_NONE][0]['value'] = $u->fullname;
$account->field_about[LANGUAGE_NONE][0]['value'] = $u->about;
$account->timezone = variable_get('date_default_timezone', '');
print_r($account);
  $usr = user_save($account);
    if($u->openid<>''){
    	  $authmaps=array();
  $authmaps['auth_openid']=$u->openid;
     user_set_authmaps($account,$authmaps);
    }
  //  $num_updated = db_update('users')->fields(array('pass' => user_hash_password("teston"),  ))->condition('uid', $usr->uid, '=')->execute();
_user_mail_notify("register_admin_created",$usr);
drupal_set_message(t("User @title added",array("title"=>$u->name)));
echo t("User @title added",array("@title"=>$u->name))."<br/>";
	}

	/////////////// fin add////////
	/////////////////// edit users////////////////////
	else {
	$existing = user_load($uid);
$edit = (array) $existing;
$edit['mail']=$u->email;

$edit['field_display_name']['und'][0]['value'] = $u->display_name;
$edit['field_idutilisateur']['und'][0]['value'] = $u->id;
$edit['field_fullname']['und'][0]['value'] = $u->fullname;
$edit['field_about']['und'][0]['value'] = $u->about;
user_save($existing, $edit);
  $num_updated = db_update('users')
  ->fields(array(
    'pass' => user_hash_password($u->name),
  ))
  ->condition('uid', $uid, '=')
  ->execute();
drupal_set_message(t("User @title edited",array("@title"=>$u->name)));
echo t("User @title edited",array("@title"=>$u->name))."<br/>";

	}
	}
	}
