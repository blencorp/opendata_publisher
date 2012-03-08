<?php

/**
 * @file
 * OpenData Module Helper Functions
 */

/**
 * Builds an array of fields selected to be exportable as csv.
 *
 * @param string $dtname
 *   The name of the table to use as source.
 *
 * @param array $cols
 *   An array of column names.
 *
 * @param array $sfields
 *   An array of fields selected on summary page.
 *
 * @param string $pkey
 *   The primary key on the table.
 *
 * @return array
 *   A fields array for views.
 */
function _opendata_publisher_build_csv_fields($dtname, $cols, $sfields, $pkey) {
  // Instantiate an array to hold the final fields structure.
  $csv_fields = array();

  /*
    IMPORTANT!
    set the table key as the first field
    to pass the right value to the detail page
  */
  $csv_fields[$pkey] = array(
   'id' => $pkey,
   'table' => $dtname,
   'field' => $pkey,
   'label' => _opendata_publisher_formatter($pkey, 'space'),
   'relationship' => 'none',
   'separator' => '',
   'exclude' => 1,
  );
  $csv_fields[$pkey]['display_order'] = 0;

  // Find out the table key column name.
  $pkey_column = $cols[$pkey];

  $temp_display_order = 999;

  foreach ($cols as $colname) {
    // Add the field_ prefix to the column name.
    $selected_colname = 'field_selected_' . $colname;
    $exportable_colname = 'field_exportable_' . $colname;
    $sort_colname = 'field_display_order_' . $colname;

    // If the field is selected, build the view field.
    if ($sfields[$selected_colname] == 1 || $sfields[$exportable_colname] == 1) {
      $csv_fields[$colname] = array(
        'id' => $colname,
        'table' => $dtname,
        'field' => $colname,
        'label' => _opendata_publisher_formatter($colname, 'space'),
        'exclude' => 0,
        'relationship' => 'none',
        'alter' => array(
          'make_link' => 1,
          'path' => $url,
        ),
      );

      if ($sfields[$sort_colname] != '' or $sfields[$sort_colname] > 0) {
        $csv_fields[$colname]['display_order'] = $sfields[$sort_colname];
      }
      else {
        $csv_fields[$colname]['display_order'] = $temp_display_order;
        $temp_display_order++;
      }
    }
  }

  // Sort the display order.
  $csv_fields = _opendata_publisher_sort_display_order($csv_fields, 1);

  return $csv_fields;
}

/**
 * This function used to get the title field that is selected from
 * the detail page creation page's title radio button.
 * It has since been deprecated and replaced by a drop down list.
 *
 * @param array $colnames
 *   An array of column names.
 *
 * @param array $fields
 *   An array of fields selected on summary page.
 *
 * @return string
 *   The title field.
 */
function _opendata_publisher_get_title_field($colnames, $fields) {
  $tfield = 'None';

  // Instantiate a flag to see if a title field is selected.
  $is_titled = FALSE;

  foreach ($colnames as $colname) {
    $title_colname = 'field_detail_title_' . $colname;

    if ($fields[$title_colname] == '0') {
      $tfield = $colname;
      $is_titled = TRUE;
    }
  }

  // If title not defined, select first field.
  if ($is_titled == FALSE) {
    $counter = 0;
    foreach ($colnames as $colname) {
      if ($counter > 2) {
        $tfield = $colname;
        break;
      }
      $counter++;
    }
  }

  return $tfield;
}

/**
 * Creates an array to be used for views display.
 *
 * @param string $a1
 *   The database table name to use as source.
 *
 * @param string $a2
 *   The latitude field.
 *
 * @param string $a3
 *   The longitude field.
 *
 * @param string $a4
 *   The title field.
 *
 * @param string $a5
 *   The subtitle field.
 *
 * @param string $a6
 *   The primary key column name.
 *
 * @param string $a7
 *   The URL for detail page link.
 *
 * @param string $a8
 *   The detail page title.
 *
 * @return array
 *   An array of fields in views format.
 */
function opendata_publisher_get_map_fields($a1, $a2, $a3, $a4, $a5, $a6, $a7, $a8) {
  $a1 = $data_table_name;
  $a2 = $lat;
  $a3 = $lon;
  $a4 = $title;
  $a5 = $subtitle;
  $a6 = $table_key;
  $a7 = $detail_url;
  $a8 = $detail_title;

  // Instantiate an array to hold the final fields structure.
  $map_fields = array();

  $map_fields[$table_key . '_arg'] = array(
    'id' => $table_key . '_arg',
    'table' => $data_table_name,
    'field' => $table_key,
    'label' => $table_key . '_arg',
    'relationship' => 'none',
    'separator' => '',
    'exclude' => 1,
  );

  $map_fields[$detail_title . '_arg'] = array(
    'id' => $detail_title . '_arg',
    'table' => $data_table_name,
    'field' => $detail_title,
    'label' => $detail_title . '_arg',
    'relationship' => 'none',
    'separator' => '',
    'exclude' => 1,
  );

  $map_fields[$lat] = array(
    'id' => $lat,
    'table' => $data_table_name,
    'field' => $lat,
    'label' => _opendata_publisher_formatter($lat, 'space'),
    'relationship' => 'none',
    'separator' => '',
    'exclude' => 1,
  );

  $map_fields[$lon] = array(
    'id' => $lon,
    'table' => $data_table_name,
    'field' => $lon,
    'label' => _opendata_publisher_formatter($lon, 'space'),
    'relationship' => 'none',
    'separator' => '',
    'exclude' => 1,
  );

  $map_fields[$title] = array(
    'id' => $title,
    'table' => $data_table_name,
    'field' => $title,
    'relationship' => 'none',
    'separator' => '',
    'exclude' => 1,
    'label' => _opendata_publisher_formatter($title, 'space'),
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 1,
      'path' => $detail_url,
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'override' => array(
      'button' => 'Use default',
    ),
  );

  $map_fields[$subtitle] = array(
    'id' => $subtitle,
    'table' => $data_table_name,
    'field' => $subtitle,
    'label' => _opendata_publisher_formatter($subtitle, 'space'),
    'exclude' => 0,
    'relationship' => 'none',
    'separator' => '',
  );

  return $map_fields;
}

/**
 * Builds a views filter fields array.
 *
 * @param string $dbt
 *   The name of the table to use as source.
 *
 * @param array $colnames
 *   An array of column names.
 *
 * @param array $s_fields
 *   An array of fields selected on summary page.
 *
 * @param array $url
 *   A url.
 *
 * @param string $table_key
 *   The primary key on the table.
 *
 * @return array
 *   A fields array for views.
 */
function opendata_publisher_build_filter_fields($dbt, $colnames, $s_fields, $url, $table_key) {
  // Holds the built out views fields.
  $s_selected_fields = array();

  // Find out the table key column name.
  $table_key_column = $colnames[$table_key];

  // Temp display order.
  $temp_display_order = 999;

  foreach ($colnames as $colname) {
    // Add the field_ prefix to the column name.
    $selected_colname = 'field_selected_' . $colname;
    $sort_colname = 'field_display_order_' . $colname;

    // If the field is selected, build the view field.
    if ($s_fields[$selected_colname] == 1) {
      $s_selected_fields[$colname] = array(
        'operator' => 'contains',
        'value' => '',
        'group' => '',
        'exposed' => TRUE,
        'expose' => array(
          'use_operator' => 0,
          'operator' => $colname . '_op',
          'identifier' => $colname,
          'label' => _opendata_publisher_formatter($colname, "space"),
          'optional' => 1,
          'remember' => 0,
        ),
        'case' => 0,
        'id' => $colname,
        'table' => $dbt,
        'field' => $colname,
        'label' => _opendata_publisher_formatter($colname, 'space'),
      );

      if ($s_fields[$sort_colname] != '' or $s_fields[$sort_colname] > 0) {
        $s_selected_fields[$colname]['display_order'] = $s_fields[$sort_colname];
      }
      else {
        $s_selected_fields[$colname]['display_order'] = $temp_display_order;
        $temp_display_order++;
      }
    }
  }

  // Sort the display order.
  $s_selected_fields = _opendata_publisher_sort_display_order($s_selected_fields, 1);

  return $s_selected_fields;
}

/**
 * Builds a views summary fields array.
 *
 * @param string $data_table_name
 *   The name of the table to use as source.
 *
 * @param array $colnames
 *   An array of column names.
 *
 * @param array $s_fields
 *   An array of fields selected on summary page.
 *
 * @param array $url
 *   A url.
 *
 * @param string $table_key
 *   The primary key on the table.
 *
 * @param string $detail_title
 *   The primary key on the table.
 *
 * @return array
 *   A fields array for views.
 */
function _opendata_publisher_build_view_summary_fields($data_table_name, $colnames, $s_fields, $url, $table_key, $detail_title) {

  // Holds the built out views fields.
  $s_selected_fields = array();

  $s_selected_fields[$table_key . '_arg'] = array(
      'label' => $table_key . '_arg',
      'id' => $table_key . '_arg',
      'table' => $data_table_name,
      'field' => $table_key,
      'relationship' => 'none',
      'separator' => '',
      'exclude' => 1,
  );
  $s_selected_fields[$table_key . '_arg']['display_order'] = '-2';

  // Set the detail page link title as the second field.
  $s_selected_fields[$detail_title . '_arg'] = array(
      'label' => $detail_title . '_arg',
      'id' => $detail_title,
      'table' => $data_table_name,
      'field' => $detail_title,
      'relationship' => 'none',
      'separator' => '',
      'exclude' => 1,
  );
  $s_selected_fields[$detail_title . '_arg']['display_order'] = '-1';

  // Temp order.
  $temp_display_order = 999;

  foreach ($colnames as $colname) {
    // Add the field_ prefix to the column name.
    $selected_colname = 'field_selected_' . $colname;
    $sort_colname = 'field_display_order_' . $colname;

    // If the field is selected, build the view field.
    if ($s_fields[$selected_colname] == 1) {
      $s_selected_fields[$colname] = array(
        'id' => $colname,
        'table' => $data_table_name,
        'field' => $colname,
        'label' => _opendata_publisher_formatter($colname, 'space'),
        'exclude' => 0,
        'relationship' => 'none',
        'alter' => array(
          'make_link' => 0,
          'path' => $url,
        ),
      );

      if ($s_fields[$sort_colname] != '' or $s_fields[$sort_colname] > 0) {
          $s_selected_fields[$colname]['display_order'] = $s_fields[$sort_colname];
      }
      else {
          $s_selected_fields[$colname]['display_order'] = $temp_display_order;
          $temp_display_order++;
      }
    }
  }

  // Sort the display order.
  $s_selected_fields = _opendata_publisher_sort_display_order($s_selected_fields, 1);

  // Mark the first element as a link.
  $i = 0;
  foreach ($s_selected_fields as $key => $value) {
    if ($i == 2) {
      $s_selected_fields[$key]['alter']['make_link'] = 1;
    }
    $i++;
  }

  return $s_selected_fields;
}

/**
 * Builds a views fields array for detail page.
 *
 * @param string $a1
 *   The name of the table to use as source.
 *
 * @param array $a2
 *   An array of column names.
 *
 * @param array $a3
 *   An array of fields selected on summary page.
 *
 * @param string $a4
 *   The primary key on the table.
 *
 * @param bool $a5
 *   A url.
 *
 * @return array
 *   A fields array for views.
 */
function _opendata_publisher_build_detail_view_fields($a1, $a2, $a3, $a4, $a5) {
  $data_table_name = $a1;
  $colnames = $a2;
  $sfields = $a3;
  $table_key = $a4;
  $do_map = $a5;

  // Temp display order.
  $dorder = -100;

  // Instantiate array to hold selected detail fields.
  $dfields = array();

  $dfields[$table_key] = array(
    'id' => $table_key,
    'table' => $data_table_name,
    'field' => $table_key,
    'label' => _opendata_publisher_formatter($table_key, 'space'),
    'exclude' => 0,
    'relationship' => 'none',
    'separator' => '',
    'exclude' => 1,
  );
  $dfields[$table_key]['display_order'] = $dorder;

  $temp_display_order = 999;

  // First need to add the badge fields to the top of the list.
  foreach ($colnames as $colname) {
    // Add the field_ prefix to the column name.
    $badge_colname = 'field_badge_' . $colname;
    $sort_colname = 'field_display_order_' . $colname;

    // If a field is marked as "badge," add it to the array.
    if ($sfields[$badge_colname] == 1) {
      $dfields[$colname] = array(
        'id' => $colname,
        'table' => $data_table_name,
        'field' => $colname,
        'label' => $colname,
        'exclude' => 1,
        'relationship' => 'none',
      );

      $dfields[$colname]['display_order'] = $dorder;
      $dorder++;
    }
  }

  // Obtain a list of columns and save them into $dfield.
  foreach ($dfields as $key => $row) {
    $dfield[$key] = $row['display_order'];
  }

  /*
  array_multisort(array_to_sort, sort_option, more_options,...)
  it returns TRUE on success
  we are adding $dfields so it sorts by the common key
  */
  array_multisort($dfield, SORT_ASC, $dfields);

  // Create a custom field to contain the badge fields.
  if (count($dfields) > 0) {
    $strtext = '<div class="badge-container">';

    // Add the badge fields to the custom field.
    foreach ($dfields as $bfield) {
      $badge_colname = 'field_badge_' . $bfield['label'];
      $break_colname = 'field_line_break_' . $bfield['label'];
      $sort_colname = 'field_display_order_' . $bfield['label'];

      // Exclude non-badge fields if they exist.
      if ($sfields[$badge_colname] == 1) {
        $strtext .= '<div class="badge">[' . $bfield['label'] . ']</div>';

        // If the badge field is marked with line break, we add it here.
        if ($sfields[$break_colname] == 1) {
          $strtext .= '<div class="clear"></div>';
        }
      }
    }

    $strtext .= '</div>';

    // Construct global custom field.
    $colname = "nothing";
    $dfields[$colname] = array(
      'label' => '',
      'alter' => array(
        'text' => $strtext,
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'id' => $colname,
      'table' => 'views',
      'field' => $colname,
      'relationship' => 'none',
    );

    // We want this custom field to be right under the badge fields.
    $dfields[$colname]['display_order'] = $dorder;
  }

  // Construct all non-badge fields by excluding fields marked as badge.
  foreach ($colnames as $colname) {
    // Add the field_ prefix to the column name.
    $selected_colname = 'field_selected_' . $colname;
    $sort_colname = 'field_display_order_' . $colname;
    $break_colname = 'field_line_break_' . $colname;
    $badge_colname = 'field_badge_' . $colname;

    // Exclude badge fields.
    if ($sfields[$badge_colname] != 1) {
      if ($sfields[$selected_colname] == 1) {
        if ($sfields[$break_colname] == 1) {
          $strbreak = '<div class="clear"></div>';
          $strstyle = '<div class="detail wide">';
        }
        else {
          $strbreak = '';
          if ($do_map == 1) {
            $strstyle = '<div class="detail">';
          }
          else {
            $strstyle = '<div class="detail-wide">';
          }
        }

        // Construct custom text field.
        $display_colname = _opendata_publisher_formatter($colname, 'space');
        $strtext = $strstyle;
        $strtext .= '<strong>' . $display_colname . '</strong>';
        $strtext .= '<p>[' . $colname . ']</p>';
        $strtext .= '</div>';
        $strtext .= $strbreak;

        $dfields[$colname] = array(
            'id' => $colname,
            'table' => $data_table_name,
            'field' => $colname,
            'label' => '',
            'exclude' => 0,
            'relationship' => 'none',
            'alter' => array(
                'alter_text' => 1,
                'text' => $strtext,
            ),
        );

        if ($sfields[$sort_colname] != '' or $sfields[$sort_colname] > 0) {
          $dfields[$colname]['display_order'] = $sfields[$sort_colname];
        }
        else {
          $dfields[$colname]['display_order'] = $dorder;
          $dorder++;
        }
      }
    }
  }

  // Sort the display order.
  $dfields = _opendata_publisher_sort_display_order($dfields);

  return $dfields;
}

/**
 * Sorts a multidimensional array.
 *
 * @param array $selected_fields
 *   The name of the table to use as source.
 *
 * @param string $is_summary
 *   An array of column names.
 *
 * @return array
 *   A sorted array.
 */
function _opendata_publisher_sort_display_order($selected_fields, $is_summary = 0) {

  foreach ($selected_fields as $key => $row) {
    $selected_field[$key] = $row['display_order'];
  }

  array_multisort($selected_field, SORT_ASC, $selected_fields);

  return $selected_fields;
}

/**
 * Builds a style array for views display.
 *
 * @param array $colnames
 *   An array of selected columns.
 *
 * @param array $fields
 *   An array of fields.
 *
 * @return array
 *   A style array.
 */
function _opendata_publisher_build_style_options($colnames, $fields) {
  $style_options = array();
  $style_options_columns = array();
  $style_options_info = array();

  foreach ($colnames as $colname) {
    // Add the field_ prefix to the column name.
    $selected_colname = 'field_selected_' . $colname;
    $sortable_colname = 'field_sortable_' . $colname;

    // If the field is selected, build the view field.
    if ($fields[$selected_colname] == 1) {
      // Add it to the columns.
      $style_options_columns[$colname] = $colname;

      // Add it to the info.
      if ($fields[$sortable_colname] == 1) {
        $style_options_info[$colname] = array('sortable' => 1, 'separator' => '');
      }
      else {
        $style_options_info[$colname] = array();
      }
    }
  }

  $style_options = array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 1,
    'order' => 'asc',
    'columns' => $style_options_columns,
    'info' => $style_options_info,
    'default' => -1,
  );

  return $style_options;
}

/**
 * A helper function to build the fields of the columns
 *
 * @param string $tableid
 *   Selected table ID
 *
 * @return array
 *   A form array
 */
function _opendata_publisher_build_summary_listing($tableid) {
  $form['summary_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields to Include on Summary Listings'),
    '#tree' => TRUE,
    '#prefix' => '<div class="opendata-summary-fields">',
    '#suffix' => '</div>',
  );

  // Check all checkbox.
  $form['summary_fields']['checkall'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check all fields'),
    '#attributes' => array(
      'onclick' => 'checkall("summary_fields_checkbox")',
      'class' => 'checkall_checkbox',
    ),
    '#prefix' => '<div class="clear-fields">',
    '#suffix' => '</div>',
  );
  $form['summary_fields']['lbl_display_fields'] = array(
      '#type' => 'item',
      '#title' => t('Summary Listing Fields'),
  );
  $form['summary_fields']['lbl_display_order'] = array(
      '#type' => 'item',
      '#title' => t('Display Field'),
  );
  $form['summary_fields']['lbl_allow_search'] = array(
      '#type' => 'item',
      '#title' => t('Allow Search?'),
  );
  $form['summary_fields']['lbl_sortable'] = array(
      '#type' => 'item',
      '#title' => t('Sortable?'),
  );
  $form['summary_fields']['lbl_exportable'] = array(
      '#type' => 'item',
      '#title' => t('Exportable?'),
  );

  // Build the columns.
  $colnames = array();
  $colnames = _opendata_publisher_get_fields($tableid);

  // Build the fields.
  $fields = array();

  foreach ($colnames as $colname) {
    $fields[$colname] = array(
      'id' => $colname,
      'table' => $rawtablename,
      'field' => $colname,
      'label' => $colname,
      'exclude' => 0,
      'relationship' => 'none',
    );

    // Generate the table fields checkboxes.
    $selected = 'field_selected_' . $colname;
    $form['summary_fields'][$selected] = array(
      '#type' => 'checkbox',
      '#title' => t($colname),
      '#attributes' => array(
        'class' => 'summary_fields_checkbox',
      ),
    );

    // Generate Sort order fields.
    $display_order = 'field_display_order_' . $colname;
    $form['summary_fields'][$display_order] = array(
      '#type' => 'textfield',
      '#size' => 10,
    );

    // Generate filter options.
    $filter_options = 'field_filter_' . $colname;
    $form['summary_fields'][$filter_options] = array(
      '#type' => 'select',
      '#options' => array(
          'na' => '',
          'textfield' => 'Text Search',
          'select' => 'Dropdown',
          'checkbox' => 'Checkboxes',
      ),
    );

    // Generate the sortable checkboxes.
    $sortable = 'field_sortable_' . $colname;
    $form['summary_fields'][$sortable] = array('#type' => 'checkbox', '#label' => 'Sortable');

    // Generate exportable checkboxes.
    $exportable = 'field_exportable_' . $colname;
    $form['summary_fields'][$exportable] = array(
      '#type' => 'checkbox',
      '#label' => 'exportable',
    );
  }

  return $form;
}

/**
 * A helper function to build the fields of the columns
 *
 * @param string $tableid
 *   Selected table ID
 *
 * @return array
 *   A form array
 */
function _opendata_publisher_build_detail_listing($tableid) {

  $form['detail_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields to Include on Detail Page'),
    '#tree' => TRUE,
    '#prefix' => '<div class="opendata-detail-fields">',
    '#suffix' => '</div>',
  );

  // Check all checkbox.
  $form['detail_fields']['checkall'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check all fields'),
    '#attributes' => array(
      'onclick' => 'checkall("detail_fields_checkbox")',
      'class' => 'checkall_checkbox',
    ),
    '#prefix' => '<div class="clear-fields">',
    '#suffix' => '</div>',
  );

  // Build the columns.
  $colnames = array();
  $colnames = _opendata_publisher_get_fields($tableid);

  $form['detail_fields']['lbl_display_fields'] = array(
    '#type' => 'item',
    '#title' => t('Detail Listing Fields'),
  );
  $form['detail_fields']['lbl_display_order'] = array(
    '#type' => 'item',
    '#title' => t('Display Field'),
  );
  $form['detail_fields']['lbl_line_break'] = array(
    '#type' => 'item',
    '#title' => t('Line Break'),
  );
  $form['detail_fields']['lbl_badge'] = array(
    '#type' => 'item',
    '#title' => t('Badge'),
  );

  // Build the fields.
  $fields = array();

  foreach ($colnames as $colname) {
    $fields[$colname] = array(
      'id' => $colname,
      'table' => $rawtablename,
        'field' => $colname,
        'label' => $colname,
        'exclude' => 0,
        'relationship' => 'none',
    );

    // Generate the table fields checkboxes.
    $selected = 'field_selected_' . $colname;
    $form['detail_fields'][$selected] = array(
      '#type' => 'checkbox',
      '#title' => t($colname),
      '#attributes' => array(
        'class' => 'detail_fields_checkbox',
      ),
    );

    // Generate Sort order fields.
    $display_order = 'field_display_order_' . $colname;
    $form['detail_fields'][$display_order] = array('#type' => 'textfield', '#size' => 10);

    // Generate the line break.
    $line_break = 'field_line_break_' . $colname;
    $form['detail_fields'][$line_break] = array('#type' => 'checkbox');

    // Generate the badge field.
    $badge = 'field_badge_' . $colname;
    $form['detail_fields'][$badge] = array('#type' => 'checkbox');
  }

  return $form;
}

/**
 * A helper function for getting the columns
 *
 * @param string $table_name
 *   Table name
 *
 * @return array
 *   Column names
 */
function _opendata_publisher_get_fields($table_name) {
  $twtid = _opendata_publisher_get_twtid($table_name);
  $pk = NULL;
  $colnames = array();
  $sql = "SELECT colname, primarykey
          FROM {tw_columns}
          WHERE ignorecol=0 AND twtid = '%d'
          ORDER BY weight";
  $colresult = db_query($sql, $twtid);

  while ($colrow = db_fetch_object($colresult)) {
    $colnames[] = $colrow->colname;
    if ($colrow->primarykey) {
      $pk = $colrow->colname;
    }
  }

  return $colnames;
}

/**
 * A helper function for getting the columns
 *
 * @param string $table_name
 *   Table name
 *
 * @return string
 *   The table wizard id for the table.
 */
function _opendata_publisher_get_twtid($table_name) {
  $sql = "SELECT twtid
          FROM {tw_tables}
          WHERE tablename = '%s'";
  $twtid = db_result(db_query($sql, $table_name));

  return $twtid;
}

/**
 * A helper function for getting the primary key of a table
 *
 * @param string $tablename
 *   Table name
 *
 * @return array
 *   The columns in the table.
 */
function _opendata_publisher_get_primary_key($tablename) {
  $twtid = _opendata_publisher_get_twtid($tablename);
  $pk = NULL;
  $colnames = array();
  $sql = "SELECT colname, primarykey
          FROM {tw_columns}
          WHERE ignorecol=0 AND twtid = '%d'
          ORDER BY weight";
  $colresult = db_query($sql, $twtid);

  while ($colrow = db_fetch_object($colresult)) {
    if ($colrow->primarykey) {
      $colnames[$colrow->colname] = $colrow->colname;
    }
  }

  return $colnames;
}

/**
 * A helper function for getting the primary key of a table
 *
 * We only want to adopt tables in tw if it is not
 * already adopted.
 * This function is used by opendata_publisher_summary.
 *
 * @param string $tablename
 *   Table name
 *
 * @return bool
 *   Returns true if the table exists in table wizard.
 */
function _opendata_publisher_exists_in_tw($tablename) {
  $sql = "SELECT twtid
          FROM {tw_tables}
          WHERE tablename = '%s'";
  $result = db_result(db_query($sql, $tablename));

  if ($result) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * A helper function for getting the primary key of a table
 *
 * For each table name, we want to check if there is a primary key.
 * This function is used by _opendata_publisher_get_tables().
 *
 * @param string $tablename
 *   Table name
 *
 * @return bool
 *   Returns true if the table has a primary key.
 */
function _opendata_publisher_has_pk($tablename) {
  $inspect = schema_invoke('inspect');
  $schema = $inspect[$tablename];

  if (!$schema) {
    drupal_set_message(t(check_plain('Could not analyze !tablename'),
      array('!tablename' => $tablename)), 'error');
    return FALSE;
  }
  else {
    if (isset($schema['primary key'])) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
}

/**
 * A helper function for getting the table names
 *
 * @return array
 *   Array of table names
 */
function _opendata_publisher_get_tables() {
  $tablenames = array();
  $sql = "SHOW TABLES LIKE 'ed_%'";
  $result = db_query($sql);

  $options = array();
  while ($row = db_fetch_array($result)) {
    foreach ($row as $tablename) {
      $unprefixed = schema_unprefix_table($tablename);

      if (_opendata_publisher_has_pk($unprefixed)) {
        $options[$unprefixed] = $unprefixed;
      }
    }
  }
  return $options;
}

/**
 * A helper function for getting the table names
 *
 * @param string $tableid
 *   Selected table ID
 *
 * @return string
 *   Selected table name
 */
function _opendata_publisher_get_tablename($tableid) {
  $tablename = $tableid;
  return $tablename;
}

/**
 * Builds the description field.
 *
 * @param string $more_info_url
 *   The URL for the more_info link.
 *
 * @param string $desc_brief
 *   A brief description text.
 *
 * @param string $desc_extended
 *   An extended description text.
 *
 * @return array
 *   An array of the description field for views.
 */
function _opendata_publisher_build_desc_fields($more_info_url, $desc_brief, $desc_extended) {
  // Instantiate an array to hold the final fields structure.
  $desc_fields = array();

  if ($more_info_url != '') {
    $desc_fields['markup'] = array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'nl2br' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'value' => '<div id="opendata-more-info"><a href="' . $more_info_url . '"><div id="opendata-more-info-link"></div></a></div>',
      'format' => '2',
      'exclude' => 0,
      'id' => 'markup',
      'table' => 'customfield',
      'field' => 'markup',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    );
  }

  $desc_fields['markup_1'] = array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'nl2br' => 1,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'value' => '<div id="desc-brief" style="padding-top: 1em;">' . $desc_brief . '</div>',
    'format' => '2',
    'exclude' => 0,
    'id' => 'markup_1',
    'table' => 'customfield',
    'field' => 'markup',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  );

  if ($desc_extended != '' && $desc_extended != NULL) {
    $desc_fields['markup_2'] = array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'nl2br' => 1,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'value' => '<div id="desc-extended" style="margin-top: 10px;">' . $desc_extended . '</div><button id="desc-button">More</button>',
      'format' => '2',
      'exclude' => 0,
      'id' => 'markup_2',
      'table' => 'customfield',
      'field' => 'markup',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    );
  }

  $desc_fields['phpcode'] = array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'nl2br' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'value' => '<script type="text/javascript">
$(document).ready(function() {
  $(\'#desc-extended\').hide();
  $(\'#desc-button\').click(function() {
    $(\'#desc-extended\').toggle(300, function() {
      if ($(\'#desc-button\').text() == \'Less\') {
        $(\'#desc-button\').text(\'More\');
      } 
      else {
        $(\'#desc-button\').text(\'Less\');
      }
    });
  });
});
</script>',
    'exclude' => 0,
    'id' => 'phpcode',
    'table' => 'customfield',
    'field' => 'phpcode',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  );

  $desc_fields['markup_3'] = array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'nl2br' => 1,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'value' => '<div style="margin-top: 0.5em"><a href="#table">Skip to table</a></div>',
    'format' => '2',
    'exclude' => 0,
    'id' => 'markup_3',
    'table' => 'customfield',
    'field' => 'markup',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  );

  return $desc_fields;
}

/**
 * A helper function for formatting strings to desired type
 *
 * @param string $val
 *   The value of the original string.
 *
 * @param string $type
 *   The type of value it is.
 *
 * @return string
 *   A formatted text string
 */
function _opendata_publisher_formatter($val, $type) {
  switch ($type) {
    case 'lower':
      $val = strtolower($val);
      break;
    case 'hyphen':
      $val = str_replace(" ", "-", strtolower($val));
      break;
    case 'html':
      nl2br($val);
      break;
    case 'underscore':
      $val = str_replace(" ", "_", strtolower($val));
      break;
    case 'space':
      $val = str_replace("_", " ", ucwords($val));
      // Split CamelCase.
      $strreg = '/([[:upper:]][[:lower:]]+)/';
      $val = preg_split($strreg, $val, NULL,
        PREG_SPLIT_DELIM_CAPTURE|PREG_SPLIT_NO_EMPTY);
      $val = implode(" ", $val);
      break;
  }

  return $val;
}
