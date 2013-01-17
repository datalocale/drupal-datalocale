<?php
/**
 * Implements settings form
 */
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
	$form['frequence'] = array(
    '#type' => 'select',
    '#title' => t('frequence'),
    '#options' => $arrvoc,
	'#default_value' => variable_get('ckan_frequencevocab')

	);
	$form['qualite'] = array(
    '#type' => 'select',
    '#title' => t('qualite'),
    '#options' => $arrvoc,
	'#default_value' => variable_get('ckan_qualitevocab')

	);
	$form['granularite'] = array(
    '#type' => 'select',
    '#title' => t('granualite'),
    '#options' => $arrvoc,
	'#default_value' => variable_get('ckan_granularitevocab')

	);
	/*      $form['categorie'] = array(
	 '#type' => 'select',
	 '#title' => t('categorie'),
	 '#options' => $arrvoc,
	 '#default_value' => variable_get('ckan_categorievocab')

	 );*/
	$form['concept'] = array(
    '#type' => 'select',
    '#title' => t('concept'),
    '#options' => $arrvoc,
	'#default_value' => variable_get('ckan_conceptvocab')

	);
	$form['geo'] = array(
    '#type' => 'select',
    '#title' => t('Geographical granularity'),
    '#options' => $arrvoc,
	'#default_value' => variable_get('ckan_geographicvocab')

	);
	$form['licence'] = array(
    '#type' => 'select',
    '#title' => t('Licence'),
    '#options' => $arrvoc,
	'#default_value' => variable_get('ckan_licencevocab')

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
/**
 * Implements _form_submit().
 */
function ckan_admin_settings_form_submit($form, &$form_state) {

	$url=$form_state['values']['url'];
	variable_set('ckan_url', $url);
	$apikey=$form_state['values']['apikey'];
	variable_set('ckan_api', $apikey);
	$tags=$form_state['values']['tags'];
	variable_set('ckan_tagsvocab', $tags);
	$frequence=$form_state['values']['frequence'];
	variable_set('ckan_frequencevocab', $frequence);
	$concept=$form_state['values']['concept'];
	variable_set('ckan_conceptvocab', $concept);

	//$categorie=$form_state['values']['categorie'];
	//variable_set('ckan_categorievocab', $categorie);

	$qualite=$form_state['values']['qualite'];
	variable_set('ckan_qualitevocab', $qualite);

	$granularite=$form_state['values']['granularite'];
	variable_set('ckan_granularitevocab', $granularite);

	$geo=$form_state['values']['geo'];
	variable_set('ckan_geographicvocab', $geo);

	$geo=$form_state['values']['licence'];
	variable_set('ckan_licencevocab', $geo);
}