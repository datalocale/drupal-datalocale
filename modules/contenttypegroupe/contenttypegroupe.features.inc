<?php
/**
 * @file
 * contenttypegroupe.features.inc
 */

/**
 * Implements hook_node_info().
 */
function contenttypegroupe_node_info() {
  $items = array(
    'groupe' => array(
      'name' => t('groupe'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Titre'),
      'help' => '',
    ),
  );
  return $items;
}
