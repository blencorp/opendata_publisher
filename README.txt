Description
-----------

OpenData Publisher is a data visualizer/publisher created by BLENCorp for the 
Department of Education. It enables administrators to quickly publish data on 
a Drupal site. 

Dependencies
------------

* feeds
* views_customfield  
* charts_graphs  
* views 
* views_groupby
* views_or
* context 
* openlayers 
* ctools 
* mapbox 
* quicktabs
* schema
* tw  

Summary page description block 
------------------------------

The description block has four fields of type "Customfield". 
Customfield is provided by the views_customfield module. This is a field that 
allows a custom text of either plain, html, or php type. 

The first field contains the html for the "More Information" button. 
The second field contains the html for the brief description block wrapped 
by <div id="desc-brief"></div tag. And the option "Convert newlines to 
HTML <br> tags" is enabled. The third field contains the extended description 
block wrapped by <div id="desc-extended"></div> tag. This field also contains 
the toggle button, which is <button id="desc-button">More</button>. The 
nl2br option is enabled here, too. The fourth field, which is of type 
"PHP code," contains the javascript for toggling the extended description 
div. The fifth field, which is of type "Markup," contains the "Skip to table" 
hyperlink text wrapped by a div tag. The only material that requires CSS 
markup in the main Data_Ed_Gov.css file is the "More Information" image 
button. 

Detail page title generation
----------------------------

The detail page title is constructed by _opendata_get_title_field, which 
is defined in opendata.helper.inc. This function will look for a field 
set as a title field from the detail field list. If a title field is not 
selected, it will set the first selected field as the title field.

Summary page field generation 
-----------------------------

In opendata.helper.inc, the function _opendata_build_view_summary_fields 
creates the columns as well as the appropriate link to the detail page. 
All fields are initially tagged with a path attribute and this attribute 
is later extracted from all fields except the first column by the function 
_opendata_sort_display_order.

TODO
----

- Save metadata to database
- On edit, show all relevant components (views, OL presets, etc)
- On delete, remove all components

Maintainers
-----------
Mike Endale (BLEN Corp) http://drupal.org/user/1194188 
Henok Mikre (BLEN Corp) http://drupal.org/user/1015742 
