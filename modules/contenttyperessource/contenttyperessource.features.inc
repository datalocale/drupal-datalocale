<?php
/**
 * @file
 * contenttyperessource.features.inc
 */

/**
 * Implements hook_node_info().
 */
function contenttyperessource_node_info() {
  $items = array(
    'ckan_ressources' => array(
      'name' => t('ckan ressources'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
