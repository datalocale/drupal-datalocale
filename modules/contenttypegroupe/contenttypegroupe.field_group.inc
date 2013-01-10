<?php
/**
 * @file
 * contenttypegroupe.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function contenttypegroupe_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_infogroupe2|node|groupe|default';
  $field_group->group_name = 'group_infogroupe2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'groupe';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'infogroupe2',
    'weight' => '3',
    'children' => array(
      0 => 'field_grname',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'infogroupe2',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_infogroupe2|node|groupe|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_infogroupe|node|groupe|form';
  $field_group->group_name = 'group_infogroupe';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'groupe';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'infogroupe',
    'weight' => '12',
    'children' => array(
      0 => 'field_gcodepostal',
      1 => 'field_gcountry',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_infogroupe|node|groupe|form'] = $field_group;

  return $export;
}
