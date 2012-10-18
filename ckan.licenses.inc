<?php

/**
 * Licenses return ckan licenses in an array
 *
 */
function ckan_licenses(){

	$ckan = ckan_ckan();
	try {
		$licenses = $ckan->getLicenseList();
		// print_r($licenses);
		if($licenses)foreach($licenses as $k=>$g){
			//if($i<=10)
			$arrlic[$g->id]['title']=$g->title;
			$arrlic[$g->id]['url']=$g->url;
			// echo ckan_create_license($g);$i++;

		}
		return $arrlic;
	}
	catch (CkanException $e){
		drupal_set_message($e->getMessage(), "error list licenses");
		drupal_set_title("error list licenses");
		return "error list licenses";
	}

}

