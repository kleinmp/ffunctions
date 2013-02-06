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