<?php

/**
 * @file
 * Creates the primary form for opendata_publisher.
 *
 * There used to be a require_once call to include opendata_publisher.preset.inc,
 * but we are now loading it with module_load_include right before calling
 * opendata_publisher_create_preset().
 *
 * Process:
 *  - module calls opendata_publisher_general
 *  - if param $form_state['storage']['summary'] is set, it calls
 *    opendata_publisher_summary
 *  - if param $form_state['storage']['detail'] is set, it calls
 *    opendata_publisher_detail
 *  - if param $form_state['storage']['published'] is set, it calls
 *    opendata_publisher_published
 * @param $form_state
 *    An array holding the form data
 * @return
 *    A $form array with user provided values.
 *
 * @TODO the office field should have a db source
 */

drupal_add_css(drupal_get_path('module', 'opendata_publisher') . '/opendata_publisher.css');
drupal_add_js(drupal_get_path('module', 'opendata_publisher') . '/opendata_publisher.js');
module_load_include('inc', 'opendata_publisher', 'opendata_publisher.helper');
module_load_include('inc', 'opendata_publisher', 'opendata_publisher.context');
module_load_include('inc', 'opendata_publisher', 'opendata_publisher.preset');
module_load_include('inc', 'opendata_publisher', 'opendata_publisher.display');

/**
 * Init function to create the form
 */
function opendata_publisher_general($form_state) {

  if (isset($form_state['storage']['summary']) AND !isset($form_state['storage']['detail'])) {
    return opendata_publisher_summary($form_state);
  }
  elseif (isset($form_state['storage']['detail']) AND !isset($form_state['storage']['published'])) {
    return opendata_publisher_detail($form_state);
  }
  elseif (isset($form_state['storage']['published'])) {
    return opendata_publisher_published($form_state);
  }

  // Basic Information.
  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grant Basic Information'),
    '#tree' => TRUE,
  );
  $form['details']['data_table'] = array(
    '#type' => 'select',
    '#title' => t('Data Table (Dont see your table? <a href="?refresh=1">Refresh</a> the list)'),
    '#options' => _opendata_publisher_get_tables(),
    '#description' => t('Please select the data table'),
  );
  $form['details']['office'] = array(
    '#type' => 'select',
    '#title' => t('Office'),
    '#options' => array(
      'OII' => 'Office of Innovation and Improvement',
      'OESE' => 'Office of Elementary and Secondary Education',
      'OPEPD' => 'Office of Planning, Evaluation and Policy Development',
      'OPE' => 'Office of Post-Secondary Education',
      'IES' => 'Institute of Education Sciences',
      'FSA' => 'Federal Student Aid',
    ),
    '#description' => t('Please select the office'),
  );
  $form['details']['fiscal_year'] = array(
    '#type' => 'select',
    '#title' => t('Fiscal Year'),
    '#options' => array(
      '2010' => '2010',
      '2011' => '2011',
      '2012' => '2012',
      '2013' => '2013',
    ),
    '#description' => t('Please select the fiscal year'),
  );
  $form['details']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Please enter the Title'),
  );
  $form['details']['desc_brief'] = array(
    '#type' => 'textarea',
    '#title' => t('Brief Description'),
    '#description' => t('Please enter a brief description of the dataset'),
    '#rows' => 3,
  );
  $form['details']['desc_extended'] = array(
    '#type' => 'textarea',
    '#title' => t('Extended Description'),
    '#description' => t('Please full description of the dataset'),
    '#rows' => 4,
  );
  $form['details']['more_info_url'] = array(
    '#type' => 'textfield',
    '#title' => t('More Info URL'),
    '#description' => t('Please enter the more info URL'),
  );
  $form['next_summary'] = array(
     '#type' => 'submit',
     '#value' => 'Next',
  );

  if ($_GET['refresh'] == 1) {
    drupal_set_message(t('Table Refreshed'));
  }

  return $form;
}

/**
 * Creates the form that defines the listing (summary) page.
 *
 * @TODO write helper functions to guess a label for each field
 * @TODO we also need a function to guess the field for each map option field
 * @TODO use a helper function to guess the column names for the summary field
 *
 * Process:
 *  - creates an array called $step_1 to display three items from the first form
 *    - grant title
 *    - data table name
 *    - fiscal year
 *
 *  - it calls helper function _opendata_publisher_get_tablename($tablename) to
 *    get the actual table name of the data selected at step_1
 *  - it calls helper function _opendata_publisher_get_fields to populate the
 *    field "Table Key" with the available columns in the data table selected.
 *  - it calls the helper function _opendata_publisher_build_summary_listing()
 *    to populate the form with available fields from the data table (passing
 *    the name of the data table selected at step_1
 */
function opendata_publisher_summary($form_state) {
  $step_1 = array();
  // Get title of the dataset.
  $step_1['title'] = $form_state['storage']['general_values']['details']['title'];
  // Get dataset table.
  $step_1['data_table'] = $form_state['storage']['general_values']['details']['data_table'];
  // Get dataset table.
  $step_1['fiscal_year'] = $form_state['storage']['general_values']['details']['fiscal_year'];

  // It is at this stage that we need to adopt the table in question
  // into table wizard, if it is not already adopted, that is.
  $tablename = $step_1['data_table'];
  $is_adopted = _opendata_publisher_exists_in_tw($tablename);
  if (!$is_adopted) {
    tw_add_tables($step_1['data_table']);
  }

  // Show grant title, data table, fiscal year -- for reference.
  $form['r_grant_title'] = array(
    '#type' => 'item',
    '#title' => t('Grant Title - ' . $step_1['title']),
  );
  $form['r_data_table'] = array(
    '#type' => 'item',
    '#title' => t('Data Table - ' . $step_1['data_table']),
  );
  $form['r_fiscal_year'] = array(
    '#type' => 'item',
    '#title' => t('Fiscal Year - ' . $step_1['fiscal_year']),
  );

  // Other summary related fields.
  $form['summary']['table_key'] = array(
    '#type' => 'select',
    '#title' => t('Table Key'),
    '#options' => _opendata_publisher_get_primary_key($step_1['data_table']),
    '#description' => t('Please choose the unique column'),
  );
  $form['summary']['page_limit'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => "Page Limit",
    '#default_value' => "25",
    '#description' => t('Please enter the page limit. Enter 0 for unlimited'),
  );
  $form['summary']['csv_download'] = array(
    '#type' => 'checkbox',
    '#title' => t('CSV Download'),
    '#description' => t('Please select to allow visitors to download raw CSV file.'),
    '#default_value' => variable_get('TRUE', 1),
  );

  // Append the columns based on the selected data table.
  $form[] = _opendata_publisher_build_summary_listing($step_1['data_table']);

  // Map field set.
  $form['summary_map'] = array(
    '#type' => 'fieldset',
    '#title' => t('Map Options'),
    '#tree' => TRUE,
  );
  $form['summary_map']['map_option'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Map'),
    '#description' => t('Please check if you would like to show a map.'),
  );
  $form['summary_map']['map_title'] = array(
    '#type' => 'select',
    '#title' => t('Title'),
    '#options' => _opendata_publisher_get_fields($step_1['data_table']),
    '#description' => t('Please choose the column that you would like to use as the title of the map.'),
  );
  $form['summary_map']['map_sub_title'] = array(
    '#type' => 'select',
    '#title' => t('Sub Title'),
    '#options' => _opendata_publisher_get_fields($step_1['data_table']),
    '#description' => t('Please choose the column that you would like to use as the sub title of the map.'),
  );
  $form['summary_map']['map_lat'] = array(
    '#type' => 'select',
    '#title' => t('Latitude'),
    '#options' => _opendata_publisher_get_fields($step_1['data_table']),
    '#description' => t('Please choose the column that holds the latitude value.'),
  );
  $form['summary_map']['map_long'] = array(
    '#type' => 'select',
    '#title' => t('Longitude'),
    '#options' => _opendata_publisher_get_fields($step_1['data_table']),
    '#description' => t('Please choose the column that holds the longitude value.'),
  );

  $form['next_detail'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

/**
 * Creates the form that defines the detail page.
 *
 * @TODO write helper functions to guess a label for each field
 * @TODO we also need a function to guess the field for each map option field
 * @TODO use a helper function to guess the column names for the summary field
 * @TODO combine helper function _opendata_publisher_build_summary_listing and
 *        _opendata_publisher_build_detail_listing
 *
 * Process:
 *  - creates an array called $step_2 to display three items from the first step
 *    - grant title
 *    - data table name
 *    - fiscal year
 *
 *  - it calls helper function _opendata_publisher_get_tablename($tablename) to
 *    get the actual table name of the data selected at step_1
 *  - it calls helper function _opendata_publisher_build_detail_listing to
 *    populate the form with the available fields from the data table (passing
 *    the name of the data table selected at step_1
 */
function opendata_publisher_detail($form_state) {
  $step_2 = array();

  // Get title of the dataset.
  $step_2['title'] = $form_state['storage']['general_values']['details']['title'];
  // Get dataset table.
  $step_2['data_table'] = $form_state['storage']['general_values']['details']['data_table'];
  // Get dataset table.
  $step_2['fiscal_year'] = $form_state['storage']['general_values']['details']['fiscal_year'];

  // Show grant title, data table, fiscal year -- for reference.
  $form['r_grant_title'] = array(
    '#type' => 'item',
    '#title' => t('Grant Title - ' . $step_2['title']),
  );
  $form['r_data_table'] = array(
    '#type' => 'item',
    '#title' => t('Data Table - ' . _opendata_publisher_get_tablename($step_2['data_table'])),
  );
  $form['r_fiscal_year'] = array(
    '#type' => 'item',
    '#title' => t('Fiscal Year - ' . $step_2['fiscal_year']),
  );
  $form['detail_title_field'] = array(
    '#type' => 'select',
    '#title' => t('Field to use as title of detail page'),
    '#options' => _opendata_publisher_get_fields($step_2['data_table']),
    '#description' => t('Select the field you want to use as the titlen'),
  );

  // Append the columns based on the selected data table.
  $form[] = _opendata_publisher_build_detail_listing($step_2['data_table']);

  $form['detail_map']['map_option'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Map'),
    '#description' => t('Please check if you would like to show a map.'),
  );

  $form['publish'] = array(
    '#type' => 'submit',
    '#value' => t('Publish'),
  );

  return $form;
}

/**
 * Publishes the dataset by creating a view, an OL preset, and a context
 * using the values submitted by the form.
 *
 * @TODO use this function as the control center for putting everything together
 * @TODO consider combining this into opendata_publisher_general_submit
 *
 * Process:
 *  - it calls helper function _opendata_publisher_get_tablename($tablename)
 *    to get the actual table name of the data selected at step_1
 *  - it calls helper function _opendata_publisher_build_detail_listing to
 *    populate the form with the available fields from the data table
 *    (passing the name of the data table selected at step_1
 */
function opendata_publisher_published(&$form_state) {

  // Debug.
  // dpm($form_state);
  opendata_publisher_general_views($form_state);
}

/**
 * Implements hook_form_submit().
 */
function opendata_publisher_general_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-next-summary') {
    $form_state['storage']['summary'] = TRUE;
    $form_state['storage']['general_values'] = $form_state['values'];
  }
  elseif ($form_state['clicked_button']['#id'] == 'edit-next-detail') {
    $form_state['storage']['detail'] = TRUE;
    $form_state['storage']['summary_values'] = $form_state['values'];
  }
  elseif ($form_state['clicked_button']['#id'] == 'edit-publish') {
    $form_state['storage']['published'] = TRUE;
    $form_state['storage']['published_values'] = $form_state['values'];
  }
  else {
    $general_values = $form_state['storage']['general_values'];
  }
}

/**
 *  Implements hook_default_view_views().
 *
 *  It builds the view, OL preset and context based on the user input.
 *
 *  @TODO break this function into smaller functions and deprecate it.
 *  @TODO consider combining into opendata_publisher_general_submit()
 *
 *  Helper functions used:
 *    - _opendata_publisher_get_tablename($data_table_id)
 *    - _opendata_publisher_formater($office_name)
 *    - _opendata_publisher_get_fields($data_table_id)
 *    - _opendata_publisher_build_view_summary_fields($table_name,
 *         $colnames, $summary_fields)
 *    - _opendata_publisher_build_style_options($colnames, $summary_fields)
 *    - opendata_publisher_create_preset($preset_name)
 */
function opendata_publisher_general_views(&$form_state) {

  // Debug.
  // dpm($form_state);
  // Basic data points collected from the user.
  $data_table_id = $form_state['storage']['general_values']['details']['data_table'];
  $data_table_name = _opendata_publisher_get_tablename($data_table_id);
  $office = _opendata_publisher_formatter($form_state['storage']['general_values']['details']['office'], 'lower');
  $fiscal_year = $form_state['storage']['general_values']['details']['fiscal_year'];
  $full_title = $form_state['storage']['general_values']['details']['title'];
  $title = _opendata_publisher_formatter($form_state['storage']['general_values']['details']['title'], 'hyphen');
  $desc_brief = _opendata_publisher_formatter($form_state['storage']['general_values']['details']['desc_brief'], 'html');
  $desc_extended = _opendata_publisher_formatter($form_state['storage']['general_values']['details']['desc_extended'], 'html');

  $unique_name = substr('od_' . $office . '_' . $fiscal_year . '_' . _opendata_publisher_formatter($form_state['storage']['general_values']['details']['title'], 'underscore'), 0, 30);

  $page_limit = $form_state['storage']['summary_values']['page_limit'];
  $table_key = $form_state['storage']['summary_values']['table_key'];

  $csv_download = $form_state['storage']['summary_values']['csv_download'];

  $map_title = $form_state['storage']['summary_values']['summary_map']['map_title'];
  $map_sub_title = $form_state['storage']['summary_values']['summary_map']['map_sub_title'];
  $map_lat = $form_state['storage']['summary_values']['summary_map']['map_lat'];
  $map_long = $form_state['storage']['summary_values']['summary_map']['map_long'];

  // Check if "Include Map" has been checked for summary and detail pages.
  $doMap_summary = $form_state['storage']['summary_values']['summary_map']['map_option'];
  $doMap_detail = $form_state['storage']['published_values']['map_option'];

  // Detail Fields.
  // Build a perm URL.
  $url = 'grants/' . $office . '/' . $fiscal_year . '/' . $title;

  //
  // Views.
  //
  // Set the stage for a new view.
  $view = views_new_view();

  // Define the view (this code was generated by the Export).
  $view->name = $unique_name;
  $view->description = 'View generated by OpenData module for the ' . $fical_year . ' ' . $full_title . ' Dataset';
  $view->tag = $office;
  $view->view_php = '';
  $view->base_table = $data_table_name;
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE;

  // Create 'default' display.
  $display_type = 'default';
  opendata_publisher_create_display(&$view, $display_type, &$form_state);

  // Create a 'block' display to show the description text.
  $display_type = 'desc_block';
  opendata_publisher_create_display(&$view, $display_type, &$form_state);

  // Create a 'page' display to show table listing.
  $display_type = 'page_summary';
  opendata_publisher_create_display(&$view, $display_type, &$form_state);

  // Reset display_type.
  $display_type = '';

  // Summary map: create OpenLayers preset and views display.
  if ($doMap_summary == 1) {
    $display_type = 'openlayers_data_summary';
    opendata_publisher_create_display(&$view, $display_type, &$form_state);

    // Reset display_type.
    $display_type = '';

    // Create a new OL preset.
    $preset_name = $unique_name;
    $preset_type = 'summary';
    $is_created = opendata_publisher_create_preset($preset_name, $preset_type);

    // If preset is created successfully, create the map display.
    if ($is_created) {
      $display_type = 'map_block';
      opendata_publisher_create_display(&$view, $display_type, &$form_state);
    }
    else {
      drupal_set_message(t('Error: Could not create OpenLayer preset. Please contact your Administrator.'), 'error');
    }

    // Reset flag.
    $is_created = FALSE;
  }

  // Create detail page display.
  $display_type = 'page_detail';
  opendata_publisher_create_display(&$view, $display_type, &$form_state);

    // Reset display_type.
    $display_type = '';

  // Detail map: create OpenLayers preset and views display.
  if ($doMap_detail == 1) {
    $display_type = 'openlayers_data_detail';
    opendata_publisher_create_display(&$view, $display_type, &$form_state);

    // Reset display_type.
    $display_type = '';

    // Create a new OL preset.
    $preset_name = $unique_name;
    $preset_type = 'detail';
    $dmap_created = opendata_publisher_create_preset($preset_name, $preset_type);

    // If preset is created successfully, create the map block display.
    if ($dmap_created) {
      $display_type = 'map_block_detail';
      opendata_publisher_create_display(&$view, $display_type, &$form_state);

      // Reset display_type.
      $display_type = '';

    }
    else {
      drupal_set_message(t('Error: Could not create OpenLayer preset for detail view. Please contact your Administrator.'), 'error');
    }
  }

  // Create CSV download 'block', if the csv export button is marked.
  if ($csv_download == 1) {
    $display_type = 'csv_block';
    opendata_publisher_create_display(&$view, $display_type, &$form_state);

    // Reset display_type.
    $display_type = '';
  }

  // Debug.
  // dpm($view);
  // Build summary context.
  $cname = $unique_name;
  $tag = 'OpenData Module';

  $conditions = array();
  $conditions[] = $unique_name . ':block_1';
  $conditions[] = $unique_name . ':block_2';
  $conditions[] = $unique_name . ':page_1';

  $reactions = array();
  $reactions[] = array('views', $unique_name . '-block_1', 'content_top', 0);
  $reactions[] = array('views', $unique_name . '-block_2', 'content_top', 1);
  $reactions[] = array('views', $unique_name . '-page_1', 'content_top', 2);

  // Create a new context.
  $is_created = opendata_publisher_create_context($cname, $tag, $conditions, $reactions);

  if (!$is_created) {
      drupal_set_message(t('Error: Could not create summary context. Please contact your Administrator.'), 'error');
  }

  // Reset flag.
  $is_created = FALSE;

  // Build summary context.
  if ($doMap_detail == 1) {
    $cname = $unique_name . '_detail';
    $tag = 'OpenData Module';

    $conditions = array();
    $conditions[] = $unique_name . ':page_2';

    $reactions = array();
    $reactions[] = array('views', $unique_name . '-block_3', 'right', 0);

    // Create a new context.
    $is_created = opendata_publisher_create_context($cname, $tag, $conditions, $reactions);

    if (!$is_created) {
        drupal_set_message(t('Error: Could not create summary context. Please contact your Administrator.'), 'error');
    }
  }

  // Save view.
  $view->save();

  // Make sure menu items get rebuilt as necessary.
  menu_rebuild();

  // Clear the views cache.
  cache_clear_all('*', 'cache_views');

  // Clear the page cache.
  cache_clear_all();

  // Remove this view from cache so we can edit it properly.
  views_object_cache_clear('view', $view->name);

  // Refresh openlayers so the map layer loads.
  openlayers_layers_load();

  global $base_url;
  $link = $base_url . '/' . $url;
  drupal_set_message(t('Dataset Published Successfully!'));
  drupal_set_message(t('Showtime @ !url', array('!url' => l($link, $link))));
}
