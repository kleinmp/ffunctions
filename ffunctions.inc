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
 * Convert a date string or timestamp to any given format.
 *
 * @param string $date_string
 *  A date string that is readable by php or a UNIX timestamp.
 * @param string $format
 *  A valid date format as defined in php's date function.
 * @param array $options
 *  (optional) timezone_to - The timezone for the returned date value.
 *  (optional) timezone_from - The timezone that the date_string is currently in.
 *  (optional) timestamp - Whether or not this is a UNIX timestamp.
 *
 * @return string
 *  The date in the format provided in the the given timezone(timezone_to).
 */
function ffunctions_get_formatted_date($date_string, $format = 'c', $options = array()) {
  // Combine defaults with given options.
  $options += array(
    'timezone_to' => 'UTC',
    'timezone_from' => 'UTC',
    'timestamp' => FALSE,
  );

  // Special handlng of timestamps
  if (!empty($options['timestamp'])) {
    $date_string = "@$date_string";
    $options['timezone_from'] = date_default_timezone_get();
  }
  $date = new DateTime($date_string, new DateTimeZone($options['timezone_from']));
  $date->setTimeZone(new DateTimeZone($options['timezone_to']));
  return $date->format($format);
}

/**
 * Compute the number of business days between (and including) two dates.
 *
 *  Note that this does not take holidays into account.
 *
 * @param int|string $start
 *  Preferably a data string compatible with strtotime,
 *  but can also be timestamp.
 * @param int|string $end
 *  Preferably a data string compatible with strtotime,
 *  but can also be timestamp.
 */
function ffunctions_get_business_day_count($start, $end) {
  $start = !is_int($start) ? strtotime($start) : $start;
  $end = !is_int($end) ? strtotime($end) : $end;

  $total_days = ($end - $start) / 86400 + 1;
  $first_day = date('N', $start);
  $last_day = date('N', $end);
  $full_weeks = floor($total_days / 7);
  $extra_days = fmod($total_days, 7);

  // Check if the first and last day are within the same week.
  if ($first_day <= $last_day) {
    // Subtract a day if last day is on Saturday or later.
    if ($first_day <= 6 && 6 <= $last_day) {
      $extra_days--;
    }
    // Subtract a day if last day is on Sunday.
    if ($first_day <= 7 && 7 <= $last_day) {
      $extra_days--;
    }
  }
  else {
    // We're not in the same week, so the count will
    // be different depending on the first day.
    if ($first_day == 7) {
      $extra_days--;
      if ($last_day == 6) {
        $extra_days--;
      }
    }
    else {
      // If the first day is not Sunday, then we subtract both weekend days.
      $extra_days -= 2;
    }
  }

  // Add up the the days in the full weeks and the extra days.
  $day_count = 5*$full_weeks + ($extra_days > 0 ? $extra_days : 0);
  return $day_count;
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
 * Access callback for determining if given ip matches the allowed ips.
 *
 * @param array
 *  A list of ips in a list or cidr notation (e.g. array('45.45.45.45/26'))
 * @return boolean
 *   Whether or not the ip is allowed.
 */
function ffunctions_ip_is_allowed($allowed_ips) {
  $ip = !empty($_SERVER['HTTP_X_FORWARD_FOR']) ? $_SERVER['HTTP_X_FORWARD_FOR'] : $_SERVER['REMOTE_ADDR'];
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

/**
 * Clone an object that contains nested objects (recursive in a sense).
 *
 * @param object $object
 *  The object to clone.
 *
 * @return object
 *  A copy of the object.
 */
function ffunctions_deep_clone($object) {
  return unserialize(serialize($object));
}