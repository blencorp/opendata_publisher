<?php

/**
 * @file
 * Creates OpenLayer Presets
 */


/**
 * Implements hook_form_submit().
 *
 * openlayers_preset_save takes a $style array
 * it is located on line 1095 of openlayers.module file
 *
 * A clone of hook_openlayers_presets()
 * similar code in:
 * openlayers/modules/openlayers_ui/includes/openlayers_ui.presets.inc
 * on line 464.
 *
 * @param string $preset_name
 *   a preferred name for the preset
 *
 * @param string $preset_type
 *   detail or full page preset
 *
 * @return bool
 *   true if preset was successfully created.
 */
function opendata_publisher_create_preset($preset_name, $preset_type) {
  // Make sure preset name is alpha-numeric.
  $pname = strtolower($preset_name);
  $pname = str_replace(' ', '_', $pname);

  // Save the original pname for use with layers below.
  $layername = $pname;

  if ($preset_type == 'detail') {
    $pname = $pname . '_detail';
  }

  // Get the map data array.
  $map = _opendata_publisher_build_map($pname, $preset_type, $layername);

  $preset = new stdClass();
  $preset->disabled = FALSE;
  $preset->api_version = 1;
  $preset->name = $pname;
  $preset->title = $pname;
  $preset->description = "A preset created by OpenData module.";
  $preset->data = $map;

  $result = openlayers_preset_save($preset);

  if ($result) {
    return TRUE;
  }
  else {
    $strmsg = 'Error: could not save OL preset';
    drupal_set_message(t(check_plain('Could not save OpenLayers Preset !pn.'), 
      array('!pn' => $pn)), 'error');
    return FALSE;
  }
}

/**
 * Creates the preset descriptor.
 *
 * @param string $pname
 *   a preferred name for the preset
 *
 * @param string $preset_type
 *   detail or full page preset
 *
 * @param string $layername
 *   a preferred name for the layer
 *
 * @return array
 *   an array of the layer description.
 */
function _opendata_publisher_build_map($pname, $preset_type, $layername) {
  if ($preset_type == 'detail') {
    $height = '200px';
    $centerpoint = '-101.07421874601, 39.368279147837';
    $zoom = '3';
    $dot_style = 'dped_blue_large';
    $point_zoom_level = '3';
    $layer = $layername . '_openlayers_2';
    $fullscreen = '';
  }
  else {
    $height = '500px';
    $centerpoint = '-101.07421874601, 39.368279147837';
    $zoom = '2';
    $dot_style = 'dped_blue';
    $point_zoom_level = '5';
    $layer = $layername . '_openlayers_1';
    $fullscreen = array();
  }
  $map = array(
    'width' => 'auto',
    'height' => $height,
    'image_path' => 'profiles/ngp_deped/openlayers/dark/',
    'css_path' => '',
    'proxy_host' => '',
    'center' => array(
      'initial' => array(
        'centerpoint' => $centerpoint,
        'zoom' => $zoom,
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_attribution' => array(),
      'openlayers_behavior_popup' => array(),
      'openlayers_behavior_fullscreen' => array(),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
      ),
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_zoomtolayer' => array(
        'zoomtolayer' => $layer,
        'point_zoom_level' => $point_zoom_level,
      ),
    ),
    'default_layer' => 'dped_browser_world_light_usa',
    'layers' => array(
      'dped_browser_world_light_usa' => 'dped_browser_world_light_usa',
      $layer => $layer,
    ),
    'layer_styles' => array(
      $layer => 'dped_blue',
    ),
    'layer_activated' => array(
      $layer => $layer,
    ),
    'layer_switcher' => array(),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => $dot_style,
      'select' => $dot_style,
      'temporary' => $dot_style,
    ),
    'options' => NULL,
  );

  return $map;
}
