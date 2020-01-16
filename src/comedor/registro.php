<?php

	error_reporting(0);
	
	$server = "localhost";
	$dbusername = "root";  
	$dbpassword = "root";
	
	//conectamos con el servidor
	$conectar=@mysqli_connect($server, $dbusername, $dbpassword);
	//verificamos la conexion
	if(!$conectar){
		echo"No se uudo conectar con el servidor";
	}else{
		$base=mysqli_select_db($conectar, "alergias");
		if(!$base){
			echo"No se encontro la base de datos";			
		}
	}
		
		
	if ($_POST['eliminar']){
		
		//recuperamos el id del form eliminar
		$id=$_POST['id'];
			
		$sql = "DELETE FROM alumnos WHERE codigo_alumno = '$id'";
		
		$ejecutar=mysqli_query($conectar,$sql);
		
		//verificamos la ejecucion
		if(!$ejecutar){
			echo "Error: " . $sql . "<br>" . mysqli_error($conectar);
			echo "<a href='bd.html'>Volver</a>";	
		}
		else
			echo"Alumno borrado correctamente<br><a href='bd.html'>Volver</a>";	
	}
	
	else if ($_POST['añadir']){
		//recuperamos las variables
		$id=$_POST['id'];
		$id2=$id;
		$nombre=$_POST['nombre'];
		$apellido1=$_POST['apellido1'];
		$apellido2=$_POST['apellido2'];
		$pago=$_POST['pago'];
		
		//hacemos la sentencia de sql para la tabla 'alumnos'
		$sql="INSERT INTO alumnos (codigo_alumno, Nombre, Apellido1, Apellido2, PAGO) VALUES('$id2', '$nombre', '$apellido1', '$apellido2', '$pago')";
		//ejecutamos la sentencia de sql
		$ejecutar=mysqli_query($conectar,$sql);
		
		//recuperamos las alergias, hacemos la sentencia de sql para la tabla 'tiene_alergia' y ejecutamos la sentencia de sql
		foreach($_POST['alergias'] as $selected){
			$sql2="INSERT INTO tiene_alergia (codigo_alumno, codigo_alergia) VALUES('$id2', '$selected')";
			$ejecutar2=mysqli_query($conectar,$sql2);
		}
		
		//verificamos la ejecucion
		if (!$ejecutar){
			echo "Error: " . $sql . "<br>" . mysqli_error($conectar);
			echo "<a href='bd.html'>Volver</a>";	
		}
		else if (!$ejecutar2){
			echo "Error: " . $sql2 . "<br>" . mysqli_error($conectar);
			echo "<a href='bd.html'>Volver</a>";	
		}
		else{
			echo"Datos guardados correctamente<br><a href='bd.html'>Volver</a>";
		}
	}
?>
