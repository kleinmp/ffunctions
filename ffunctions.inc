<?php

/**
 * @file
 *  Handy non-Drupal utility functions.
 */

/**
 * Log a message to the given file.
 *
 *  This is handy for debugging.
 *
 * @param string $log
 *  Path to the log file on disk.
 * @param string $message
 *  The message to log.  If this is an array or obejct, then send print_r($variable, TRUE).
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
function ffunctions_get_formatted_date($date_string, $format = 'c', $timezone_to = 'UTC', $timezone_from = 'UTC') {
  $date = new DateTime($date_string, new DateTimeZone($timezone_from));
  $date->setTimeZone(new DateTimeZone($timezone_to));
  return $date->format($format);
}

/**
 * Find a single url in an html string
 */
function ffunctions_find_url($string, $attribute = 'src', $modifiers = '') {
  $matches = array();
  $pattern = sprintf('/%s=["|\']([^"|\']+)/%s', $attribute, $modifiers);
  preg_match($pattern, $string, $matches);
  return !(empty($matches[1])) ? $matches[1] : NULL;
}

/**
* Access callback for determining if  given ip matches the allowed ips.
*
* @param array
*  A list of ips in a list or cidr notation (e.g. array('45.45.45.45/26'))
* @return boolean
*   Whether or not the ip is allowed.
*/
function ffunctions_ip_is_allowed($allowed_ips) {
  // @todo: Won't work properly with a proxy server.
  $ip = $_SERVER['REMOTE_ADDR'];
  $access = FALSE;
  if (in_array($ip, $allowed_ips, TRUE)) {
    $access = TRUE;
  }
  else {
    foreach ($allowed_ips as $range) {
      if (ffunctions_cidr_match($ip, $range)) {
        $access = TRUE;
      }
    }
  }
  return $access;
}

/**
* Given a CIDR mask and an IP address, return TRUE or FALSE if the IP address
* matches or doesn't match the CIDR mask.
* Adapted from http://stackoverflow.com/questions/594112
*/
function ffunctions_cidr_match($ip, $range) {
  list ($subnet, $bits) = explode('/', $range);
  $ip = ip2long($ip);
  $subnet = ip2long($subnet);
  // Sanity check: ip2long() returns FALSE for an invalid IP address.
  if (empty($subnet) || empty($bits) || empty($ip)) {
    return FALSE;
  }
  $mask = -1 << (32 - $bits);
  $subnet &= $mask;
  return ($ip & $mask) == $subnet;
}