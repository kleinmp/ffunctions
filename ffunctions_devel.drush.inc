<?php

/**
 * Implements hook_drush_command().
 *
 * @return
 *   An associative array describing your command(s).
 *
 * @see drush_parse_command()
 */
function ffunctions_devel_drush_command() {
  return array(
    'ffunctions-devel-setup-site' => array(
      'description' => "Setup development configuration on site",
      'drupal dependencies' => array('ffunctions_devel'),
      'aliases' => array('ff-setup', 'ffunctions_devel'),
    ),
  );
}

/**
 * Implements hook_drush_help().
 */
function ffunctions_devel_drush_help($section) {
  switch ($section) {
    case 'drush:ffunctions_devel':
      return dt("Setup development configuration on site");
  }
}

/**
 * Drush function callback to setup development configuration on site.
 */
function drush_ffunctions_devel_setup_site() {
  ffunctions_devel_setup_dev_site();
}