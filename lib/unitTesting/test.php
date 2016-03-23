#!/usr/bin/php
<?php

// some config
$dir = 'example';
$lib = 'lib';

$unitTesting = $lib . '/' . 'unitTesting.c';


(PHP_SAPI !== 'cli' || isset($_SERVER['HTTP_USER_AGENT'])) && exit;
ini_set('memory_limit','2M');
ini_set('max_execution_time', 300);


$argv = $_SERVER['argv'];
$argc = $_SERVER['argc'];

$is_clean = $argc > 1 && @$argv['1'] == 'clean';
$test_file = array();

// scan test dir
$path = realpath(__DIR__ . '/' . $dir);
$d = dir($dir);
$cwd = substr($path, strlen(__DIR__) + 1);	
while (false !== ($entry = $d->read())) {
	if($entry == '.' || $entry == '..') continue;
	
	$path = pathinfo($entry);
	if($path['extension'] == 'c'){
		$tmp = array(
			'file' => $path['basename'],
			'cwf' => $cwd . '/' . $path['basename'],
			'out' => $cwd . '/' . $path['filename'] . '.out',
			
			'source' => array(),
			'param' => array(),
		);
		
		$file_extra = $cwd . '/' . $path['filename'] . '.json';
		if(file_exists($file_extra)){
			$json = file_get_contents($file_extra);
			$json = json_decode($json, true);
			
			if(is_array($json)){
				if(isset($json["source"]))
					$tmp['source'] = $json["source"];
				if(isset($json["param"]))
					$tmp['param'] = $json["param"];
			}
		}
		
		$test_file[$path['filename']] = $tmp;
	}
}
ksort($test_file);
$d->close();


echo "# Simple Unit Testing #" . PHP_EOL . PHP_EOL;
	
// run unit test
foreach($test_file as $test){
	// check existing output
	if(file_exists($test['out'])) @unlink($test['out']);
	
	// CLEAN
	if($is_clean) continue;
	
	// compile
	$param = implode(' ', $test['param']);
	$source = implode(' ', $test['source']);
	exec("cc -o {$test['out']} {$test['cwf']} {$source} {$unitTesting} {$param}");
	
	// run
	echo '# ' . $test['file'] . PHP_EOL;
	$output = exec($test['out']) . PHP_EOL;
	
	echo str_replace('<br/>', PHP_EOL, $output);
}

if($is_clean)
	echo 'Cleaning...' . PHP_EOL;
