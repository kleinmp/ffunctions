<?php

/**
 * @file
 *  Handy non-Drupal utility functions.
 */

/**
 *
 */
function ffunctions_log_message($log, $message) {
  file_put_contents($log, date('c') . ' - ' . $message, FILE_APPEND);
}


/**
 * Convert a date string to any given format.
 *
 * @param string $date_string
 *  A date string that is readable by php.
 * @param string $format
 *  A valid date format as defined in php's date function.
 * @param string $timezone_to
 *  The timezone for the returned date value.
 * @param string $timezone_from
 *  The timezone that the date_string is currently in.
 *
 * @return string
 *  The date in the format provided in the the given timezone($timezone_to).
 */
function ffunctions_links_get_formatted_date($date_string, $format = 'c', $timezone_to = 'UTC', $timezone_from = 'UTC') {
  $date = new DateTime($date_string, new DateTimeZone($timezone_from));
  $date->setTimeZone(new DateTimeZone($timezone_to));
  return $date->format($format);
}

/**
 * Determine if an event is all day or not.
 *
 * @param array $date
 *  The loaded field date field data. This must include
 *  value, value2, timezone, timezone_db.
 * @param string $granularity
 *   (optional) The granularity of the date. Defaults to 'second'.
 *
 * @return boolean
 *  Whether or not this is an all day date.
 */
function ffunctions_links_date_is_all_day($date, $granularity = 'second') {
  if (!module_exists('date_all_day')) {
    return FALSE;
  }

  $format = 'Y-m-d H:i:s';
  $from = ffunctions_links_get_formatted_date($date['value'], $format, $date['timezone'], $date['timezone_db']);
  $to = $date['value'] == $date['value2'] ? $from :  ffunctions_links_get_formatted_date($date['value2'], $format, $date['timezone'], $date['timezone_db']);
  return date_is_all_day($from, $to, $granularity);
}
