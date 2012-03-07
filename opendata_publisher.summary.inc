<?php
/**
 * @file
 * Summary page
 */

/**
 * Renders the summary page.
 *
 * @param string $form_state
 *   An array holding the form data
 *
 * @return array
 *   A $form array with user provided values.
 *
 */
function opendata_publisher_summary(&$form_state) {
  dpm($form_state);

  // Display page 2 if $form_state['storage']['page_two'] is set.
  if (isset($form_state['storage']['page_two'])) {
    $form_state['redirect'] = 'opendata';
  }

  $step_1 = array();
  // Get title of the dataset.
  $step_1['title'] = $form_state['values']['details']['title'];
  // Get dataset table.
  $step_1['data_table'] = $form_state['values']['details']['data_table'];

  $form = _opendata_publisher_getFields($step_1['data_table']);

  // Summary Page.
  $form['summary_sort_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields to Include on Summary Listing'),
    '#tree' => TRUE,
  );
  $form['summary']['page_limit'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => "Page Limit",
    '#description' => t('Please enter the page limit. Enter 0 for unlimited'),
  );
  $form['summary']['csv_download'] = array(
    '#type' => 'radios',
    '#title' => t('CSV Download?'),
    '#options' => array('1' => t('Yes'), '0' => t('No')),
    '#description' => t('Please select yes if you want to allow visitors to download raw CSV file.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Publish'),
  );

  return $form;
}

/**
 * Gets the columns of a db table.
 *
 * @param string $tableid
 *   The id of the table as saved in table wizard.
 *
 * @return array
 *   A $form array containing the list of columns.
 *
 */
function _opendata_publisher_getFields($tableid) {
  // Grab the available columns for this table (skip any marked ignore).
  $pk = NULL;
  $colnames = array();
  $sql = "SELECT colname, primarykey
          FROM {tw_columns}
          WHERE ignorecol=0 AND twtid=%d
          ORDER BY weight";
  $colresult = db_query($sql, $tableid);

  while ($colrow = db_fetch_object($colresult)) {
    $colnames[] = $colrow->colname;
    if ($colrow->primarykey) {
      $pk = $colrow->colname;
    }
  }

  // Add our columns to the fields.
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

    // Display Sort Order Fields.
    $sort_order = 'field_sort_' . $colname;

    $form['summary_fields'][$sort_order] = array('#type' => 'textfield');
  }

  return $form;
}
