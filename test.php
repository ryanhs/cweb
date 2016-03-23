#!/usr/bin/php
<?php

// some config
$dir = 'test';
$lib = 'lib/unitTesting/lib';
$ext = '.c';
$unitTesting = $lib . '/' . 'unitTesting.c';


(PHP_SAPI !== 'cli' || isset($_SERVER['HTTP_USER_AGENT'])) && exit;
ini_set('memory_limit','8M');
ini_set('max_execution_time', 300);


$argv = $_SERVER['argv'];
$argc = $_SERVER['argc'];

$file_to_test = $argc > 1 && file_exists($dir . '/' . @$argv['1'] . $ext) ? $dir . '/' . @$argv['1'] . $ext : false;
$is_clean = $argc > 1 && @$argv['1'] == '--clean';
$test_file = array();

function cleaning(){
	global $dir;
	
	$d = dir($dir);
	while (false !== ($entry = $d->read())) {
		if($entry == '.' || $entry == '..') continue;
		if(substr($entry, -2) == '.o') @unlink($dir . '/' . $entry);
		if(substr($entry, -4) == '.out') @unlink($dir . '/' . $entry);
	}
	$d->close();
}

function parse_test_file($file){
	global $test_file, 
		   $dir, 
		   $ext;
	
	$path = realpath(__DIR__ . '/' . $dir);
	$cwd = substr($path, strlen(__DIR__) + 1);
	$path = pathinfo($path . '/' . $file);
	
	if($path['extension'] == substr($ext, 1)){
		$tmp = array(
			'file' => $path['basename'],
			'cwf' => $cwd . '/' . $path['basename'],
			'compile' => $cwd . '/' . $path['filename'] . '.o',
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

// clean
if($is_clean){
	cleaning();
	die('Cleaning...' . PHP_EOL);
}

if($file_to_test != false){
	parse_test_file($file_to_test);
}else{
	// scan test dir
	$d = dir($dir);
	while (false !== ($entry = $d->read())) {
		if($entry == '.' || $entry == '..') continue;
		parse_test_file($entry);
	}
	ksort($test_file);
	$d->close();
}

echo "# Simple Unit Testing #" . PHP_EOL . PHP_EOL;
	
// run unit test
foreach($test_file as $test){
	cleaning(); // fresh dir for every test
	
	// compile
	$param = implode(' ', $test['param']);
	$source = implode(' ', $test['source']);
	
	$oFiles = array("{$dir}/unitTesting.o", $test['compile']);
	system("gcc -Wall -c -o {$dir}/unitTesting.o {$unitTesting}");
	system("gcc -Wall -c -o {$test['compile']} {$dir}/{$test['file']} {$param}");
	
	foreach($test['source'] as $sourceFile){
		$sourceFile_pathinfo = pathinfo($sourceFile);
		$oFiles[] = "{$dir}/{$sourceFile_pathinfo['filename']}.o";
		system("gcc -Wall -c -o {$dir}/{$sourceFile_pathinfo['filename']}.o {$sourceFile} {$param}");
	}
	
	$oFile = implode(' ', $oFiles);
	system("gcc -Wall -o {$test['out']} {$oFile} {$param}");
	
	// run
	echo '# ' . $test['file'] . PHP_EOL;
	
	ob_start();
		system($test['out']) . PHP_EOL;
	$output = ob_get_clean();
	$output = str_replace('<br/>', PHP_EOL, nl2br($output));
	$output = str_replace('<br />', PHP_EOL, $output);
	
	// color & font
	$output = str_replace('<strong>', "\033[1m", $output);
	$output = str_replace('</strong>', "\033[0m", $output);
	
	$output = str_replace('<error>', "\e[0;31m", $output);
	$output = str_replace('</error>', " \033[0m", $output);
	
	
	echo str_replace("\n\n", "\n", $output) . PHP_EOL;
}


