<?php
class Ckan {
    private  $url = 'http://www.ckan.net/';
    private $apikey = 'a37d3691-312e-4d87-b95b-349f0ff31558';


  private $debugmode = FALSE;
    private $errors = array(
     100 => 'Continue',
    101 => 'Switching Protocols',
    200 => 'OK',
    201 => 'Created',
    202 => 'Accepted',
    203 => 'Non-Authoritative Information',
    204 => 'No Content',
    205 => 'Reset Content',
    206 => 'Partial Content',
    300 => 'Multiple Choices',
    301 => 'Moved Permanently',
    302 => 'Found',
    303 => 'See Other',
    304 => 'Not Modified',
    305 => 'Use Proxy',
    307 => 'Temporary Redirect',
    400 => 'Bad Request',
    401 => 'Unauthorized',
    402 => 'Payment Required',
    403 => 'Forbidden',
    404 => 'Not Found',
    405 => 'Method Not Allowed',
    406 => 'Not Acceptable',
    407 => 'Proxy Authentication Required',
    408 => 'Request Time-out',
    409 => 'Conflict (e.g. name already exists)',
    410 => 'Gone',
    411 => 'Length Required',
    412 => 'Precondition Failed',
    413 => 'Request Entity Too Large',
    414 => 'Request-URI Too Large',
    415 => 'Unsupported Media Type',
    416 => 'Requested range not satisfiable',
    417 => 'Expectation Failed',
    500 => 'Internal Server Error',
    501 => 'Not Implemented',
    502 => 'Bad Gateway',
    503 => 'Service Unavailable',
    504 => 'Gateway Time-out',
    505 => 'HTTP Version not supported',
    );


  public function __construct($url=null, $apikey=null){
    if ($url){ $this->url=$url; }
    if ($apikey) { $this->apikey = $apikey; }
  }

    private function transfer($url, $method='GET', $data=null){

        $ch = curl_init($this->url . $url);

        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_HEADER, 0);
        curl_setopt($ch, CURLOPT_TIMEOUT, 30);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1) ;
        if (in_array($method, array('POST','PUT'))) {
      if (!$this->apikey) {throw new CkanException("No API KEY.");}

      $ch_options[CURLOPT_HTTPHEADER] =  array(
      	'Authorization: '.$this->apikey,
        //'Accept: application/json',
      );
      $ch_options[CURLOPT_POSTFIELDS] = $data;
      $ch_options[CURLOPT_CUSTOMREQUEST] = 'POST';
    }
        $result = curl_exec($ch);
//print_r($result);

        $info = curl_getinfo($ch);
//print_r($info);
        curl_close($ch);
        if ($info['http_code'] != 200){
            throw new CkanException($this->error_codes["$info[http_code]"]);
        }
        if (!$result){
            throw new CkanException("No Result");
        }
        return json_decode($result);
    }

private function actiontransfer($url,$data){

$data_string = json_encode($data);

$ch = curl_init($this->url. $url);
curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1) ;
curl_setopt($ch, CURLOPT_HTTPHEADER, array(
    'Content-Type: application/json',
    'Content-Length: ' . strlen($data_string))
);

$result = curl_exec($ch);

 $info = curl_getinfo($ch);
        curl_close($ch);

        return json_decode($result);

    }


    public function search($keyword){
        $results = $this->transfer('api/search/package/?all_fields=1&q=' . urlencode($keyword));
        if (!$results->count){
            throw new CkanException("Search Error");
        }
        return $results;
    }

    public function getPackage($data){
        $package =  $this->actiontransfer('api/action/package_show',$data);

        if (!is_array($package) && !is_object($package)){
            throw new CkanException("Package Load Error");
        }
        return $package;
    }
    public function gettags($data){
            $tags =  $this->actiontransfer('api/action/tag_list',$data);

        if (!is_array($tags) && !is_object($tags)){
            throw new CkanException("Tags Load Error");
        }
        return $tags;
    }

    public function gettagsdatalocale($data){
            $tags =  $this->actiontransfer('api/action/datalocale_tag_list',$data);

        if (!is_array($tags) && !is_object($tags)){
            throw new CkanException("Tags Load Error");
        }
        return $tags;
    }
        public function tagscat($data){
            $tags =  $this->actiontransfer('api/action/datalocale_vocabulary_list',$data);

        if (!is_array($tags) && !is_object($tags)){
            throw new CkanException("Tags Load Error");
        }
        return $tags;
    }


	public function getckanusers($data){
	  $users =  $this->actiontransfer('api/action/user_list',$data);
	  if (!is_array($users) && !is_object($users)){
	    throw new CkanException("User List Error");
	  }
	  return $users;
	}
    public function getPackageList(){
        $list =  $this->transfer('api/rest/package/');
        if (!is_array($list)){
            throw new CkanException("Package List Error");
        }
        return $list;
    }

    public function getGroup($group){
        $group = $this->transfer('api/rest/group/' . urlencode($group) );
        if (!$group->name){
            throw new CkanException("Group Error");
        }
        return $group;
    }

    public function getGroupList(){
        $groupList = $this->transfer('api/rest/group/');
        if (!is_array($groupList)){
            throw new CkanException("Group List Error");
        }
        return $groupList;
    }


	public function getLicenseList(){
	  $list =  $this->transfer('api/rest/licenses');
	  if (!is_array($list)){
	    throw new CkanException("License List Error");
	  }
	  return $list;
	}

	public function getRevisionsSinceTime($time){

	   $time = strftime("%Y-%m-%dT%H:%M:%S", $time);
		$revisionList = $this->transfer('api/search/revision?since_time=' . $time, $this->no_result_means_array );
		return $revisionList;
	}

	public function getRevision($revisionID){
	  $revision = $this->transfer('api/rest/revision/' . urlencode($revisionID) );
	  return $revision;
	}

}
//api/tag_counts
class CkanException extends Exception{}
?>