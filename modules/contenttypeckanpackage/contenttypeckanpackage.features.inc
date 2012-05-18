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
      'name' => t('Ckan package'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
