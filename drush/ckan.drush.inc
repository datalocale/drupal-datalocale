<?php

/**
 * @file
 *   drush integration for apachesolr.
 */

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @see drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function ckan_drush_command() {
  $items = array();
  $items['importvocabs'] = array(
    'callback' =>'ckan_drush_importvocabs',
    'description' => dt('import vocabs'),
  );
  $items['importgroups'] = array(
    'callback' => 'ckan_drush_importgroups',
        'description' => dt('Import groups'),
     );
  $items['importusers'] = array(
    'callback' => 'ckan_drush_importusers',
    'description' => dt('Import users'),
  );
  $items['importpackages'] = array(
    'callback' => 'ckan_drush_importpackages',
    'description' => dt('Import packages'),
   
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function ckan_drush_help($section) {
  switch ($section) {
    case 'drush:importvocabs':
      return dt("Import ckan vocabs");
    case 'drush:importgroups':
      return dt("Import ckan groups");
    case 'drush:importusers':
      return dt("Import ckan users");
    case 'drush:importpackages':
      return dt("Import ckan packages");

  }
}

/**
 * Example drush command callback.
 *
 * This is where the action takes place.
 *
 * In this function, all of Drupals API is (usually) available, including
 * any functions you have added in your own modules/themes.
 *
 * To print something to the terminal window, use drush_print().
 *
 */
function ckan_drush_importvocabs() {
  module_load_include('inc', 'ckan', 'ckan.tags');
 ckan_tags();
}
function ckan_drush_importgroups() {
  module_load_include('inc', 'ckan', 'ckan.groups');
 ckan_groups();
}
function ckan_drush_importusers() {
  module_load_include('inc', 'ckan', 'ckan.users');
 ckan_users();
}
function ckan_drush_importpackages() {
  module_load_include('inc', 'ckan', 'ckan.packages');
 ckan_list();
}

