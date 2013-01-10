<?php
/**
 * @file
 * contenttyperessource.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function contenttyperessource_ctools_plugin_api() {
  return array("version" => "1");
}

/**
 * Implements hook_node_info().
 */
function contenttyperessource_node_info() {
  $items = array(
    'ckan_ressources' => array(
      'name' => t('fichiers open data'),
      'base' => 'node_content',
      'description' => t('type de contenu permettant de présenter les fichiers à télécharger ou à prévisualiser'),
      'has_title' => '1',
      'title_label' => t('Titre'),
      'help' => '',
    ),
  );
  return $items;
}
