<?php
/**
 * @file
 * contenttypeckanextrafields.features.inc
 */

/**
 * Implements hook_node_info().
 */
function contenttypeckanextrafields_node_info() {
  $items = array(
    'ckan_extrafields' => array(
      'name' => t('champs additionnels'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Titre'),
      'help' => '',
    ),
  );
  return $items;
}
