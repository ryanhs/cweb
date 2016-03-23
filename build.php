#!/usr/bin/php
<?php

(PHP_SAPI !== 'cli' || isset($_SERVER['HTTP_USER_AGENT'])) && exit;
ini_set('memory_limit','8M');
ini_set('max_execution_time', 300);

// some config
$dir = 'build';
$ext = '.c';

$argv = $_SERVER['argv'];
$argc = $_SERVER['argc'];

$file_to_test = $argc > 1 && file_exists($dir . '/' . @$argv['1'] . $ext) ? $dir . '/' . @$argv['1'] . $ext : false;
$is_clean = $argc > 1 && @$argv['1'] == '--clean';

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

// clean
if($is_clean){
	cleaning();
	die('Cleaning...' . PHP_EOL);
}


cleaning(); // fresh dir for every build
$build_files = array();

$json = json_decode(file_get_contents('build/build.json'), true);
$param = "";
if(isset($json['param'])){
	$param = implode(' ', $json['param']);
}

echo "# Build:" . PHP_EOL;

$compile_files = array();
foreach($json['source'] as $source){
	$source_pathinfo = pathinfo($source);
	
	$path = realpath(__DIR__ . '/' . $dir);
	$cwd = substr($path, strlen(__DIR__) + 1);
	$path_compile = $cwd . '/' . $source_pathinfo['filename'] . '.o';
	$compile_files[] = $path_compile;
	
	echo "  - compile: " . $source . PHP_EOL;
	system("gcc -Wall -c -g -o {$path_compile} {$source} {$param}");
}

$compiles = implode(' ', $compile_files);
echo "# build & clean: " . $json['output'] . PHP_EOL;

//system("gcc -o {$dir}/{$json['output']} {$compiles} {$param}");
@unlink("{$dir}/{$json['output']}");
system("gcc -g -o {$dir}/{$json['output']} {$compiles} {$param}");
cleaning();

//exit;

echo "# run: " . $json['output'] . PHP_EOL . PHP_EOL;
//system("./{$dir}/{$json['output']}");

echo PHP_EOL;
