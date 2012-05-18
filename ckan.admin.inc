<?php
function ckan_admin_settings_form($form_state) {
$arrvoc=getallvocabularies();

  $form['url'] = array(
    '#title' => t('Url'),
    '#type' => 'textfield',
    '#default_value' => variable_get('ckan_url'),

    '#required' => TRUE,

  );
   $form['apikey'] = array(
    '#title' => t('Api key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('ckan_api'),

  );


  $form['tags'] = array(
    '#type' => 'select',
    '#title' => t('List of vocabularies'),
    '#options' => $arrvoc,
    '#required' => TRUE,
	'#default_value' => variable_get('ckan_tagsvocab')

  );
    $form['groups'] = array(
    '#type' => 'select',
    '#title' => t('List of Vocabularies'),
    '#options' => $arrvoc,
    '#required' => TRUE,
	'#default_value' => variable_get('ckan_groupsvocab')

  );
  $form['#submit'][] = 'ckan_admin_settings_form_submit';
  return system_settings_form($form);
}
function getallvocabularies(){
$query = db_select('taxonomy_vocabulary',"l")->fields('l', array('vid','name'));
$result = $query->execute();
foreach($result as $r)$arrvoc[$r->vid]=$r->name;
return $arrvoc;
}
/**
 * Implements _form_validate().
 */
function ckan_admin_settings_form_validate($form, &$form_state) {

}

function ckan_admin_settings_form_submit($form, &$form_state) {

$url=$form_state['values']['url'];
variable_set('ckan_url', $url);
$apikey=$form_state['values']['apikey'];
variable_set('ckan_api', $apikey);
$tags=$form_state['values']['tags'];
variable_set('ckan_tagsvocab', $tags);
$groups=$form_state['values']['groups'];
variable_set('ckan_groupsvocab', $groups);
}