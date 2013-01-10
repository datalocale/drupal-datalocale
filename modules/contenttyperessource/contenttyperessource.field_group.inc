<?php
/**
 * @file
 * contenttyperessource.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function contenttyperessource_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_inforessource2|node|ckan_ressources|default';
  $field_group->group_name = 'group_inforessource2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'ckan_ressources';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'infogroupe',
    'weight' => '0',
    'children' => array(
      0 => 'body',
      1 => 'field_mimetype',
      2 => 'field_ressourceid',
      3 => 'field_size',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'infogroupe',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_inforessource2|node|ckan_ressources|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_inforessource2|node|ckan_ressources|form';
  $field_group->group_name = 'group_inforessource2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'ckan_ressources';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'ressource',
    'weight' => '5',
    'children' => array(
      0 => 'field_mimetype',
      1 => 'field_ressourceurl',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_inforessource2|node|ckan_ressources|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_inforessource|node|ckan_ressources|default';
  $field_group->group_name = 'group_inforessource';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'ckan_ressources';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'info',
    'weight' => '1',
    'children' => array(
      0 => 'field_ressourcefile',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'info',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_inforessource|node|ckan_ressources|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_inforessource|node|ckan_ressources|form';
  $field_group->group_name = 'group_inforessource';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'ckan_ressources';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Info',
    'weight' => '0',
    'children' => array(
      0 => 'body',
      1 => 'field_ressourceid',
      2 => 'field_size',
      3 => 'field_ressourcefile',
      4 => 'title',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_inforessource|node|ckan_ressources|form'] = $field_group;

  return $export;
}
