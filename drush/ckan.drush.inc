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
/*function apachesolr_drush_help($section) {
  switch ($section) {
    case 'drush:solr-delete-index':
      return dt("Used without parameters, this command deletes the entire Solr index. Used with parameters for content type, it deletes just the content types that are specified. After the index has been deleted, all content will be indexed again on future cron runs.");
    case 'drush:solr-reindex':
      return dt("Used without parameters, this command marks all of the content in the Solr index for reindexing. Used with paramters for content type, it marks just the content types that are specified. Reindexing is different than deleting as the content is still searchable while it is in queue to be reindexed. Reindexing is done on future cron runs.");
    case 'drush:solr-index':
      return dt("Reindexes content marked for (re)indexing. If you want to reindex all content or content of a specific type, use solr-reindex first to mark that content.");
    case 'drush:solr-phpclient':
      return dt("Downloads the SolrPhpClient libraray from googlecode.com. Include the optional path to an apachesolr module installation if you have more than one, or if the module is not yet enabled.");
    case 'drush:solr-search':
      return dt('Executes a search against the site\'s Apache Solr search index and returns the restults.');
  }
}*/

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

