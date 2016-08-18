<?php
	include('config.php');
	
	if(!isset($_POST['password'])
		|| !isset($_POST['hashtype'])
		|| !isset($_POST['filetype'])
	){
		header('HTTP/1.0 404 Not Found');
		echo 'Not Found';
		exit;
	};
	$password = $_POST['password'];
	$hashtype = $_POST['hashtype'];
	$filetype = $_POST['filetype'];

	if($password != $adminpassword){
		header('HTTP/1.0 403 Forbidden');
		echo 'You are forbidden!';
		exit;
	}
	
	if($hashtype != 'md5'){
		header('HTTP/1.0 404 Not Found');
		echo 'Unsupperted hashtype';
		exit;
	}

	if($filetype != 'vertexgraph' && $filetype != 'rhmem'){
		header('HTTP/1.0 404 Not Found');
		echo 'Unsupperted hashtype';
		exit;
	}
