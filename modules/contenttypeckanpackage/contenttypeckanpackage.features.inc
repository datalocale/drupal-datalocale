<?php
/**
 * @file
 * contenttypeckanpackage.features.inc
 */

/**
 * Implements hook_node_info().
 */
function contenttypeckanpackage_node_info() {
  $items = array(
    'ckan_package' => array(
      'name' => t('Fiche du jeu de données'),
      'base' => 'node_content',
      'description' => t('Cette fiche décrit le jeu de données et permet d\'accéder aux ressources liées.'),
      'has_title' => '1',
      'title_label' => t('Titre'),
      'help' => '',
    ),
  );
  return $items;
}
