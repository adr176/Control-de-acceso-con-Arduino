<?php

$server = "localhost";
$dbusername = "root";  
$dbpassword = "root"; 
 
$conexion = mysqli_connect($server, $dbusername, $dbpassword);
mysqli_select_db($conexion, "alergias");

$sql1 ="SELECT Nombre, Apellido1, Apellido2, PAGO, Descripcion
FROM
	   alergenos a
       JOIN  tiene_alergia ta ON a.idalergia = ta.codigo_alergia
       JOIN alumnos i ON ta.codigo_alumno = i.codigo_alumno
WHERE (i.codigo_alumno='" . $_GET['id']. "')";


$sql2 ="SELECT  Descripcion
FROM
	   alergenos a
       JOIN  tiene_alergia ta ON a.idalergia = ta.codigo_alergia
       JOIN alumnos i ON ta.codigo_alumno = i.codigo_alumno
       JOIN Alergia_diaria ad ON ad.Alergia_diaria=a.idalergia
WHERE (ta.codigo_alumno='" . $_GET['id']. "'AND fecha=current_date())";

// para datos del alumno
$resultado = mysqli_query($conexion,$sql1);
//para las alergias
$resultado2 = mysqli_query($conexion,$sql1);
// para las alergias diarias
$resultado3 = mysqli_query($conexion,$sql2);

while($row = mysqli_fetch_assoc($resultado)){
    $nombre = utf8_encode($row['Nombre']);
	$apellido1 = utf8_encode($row['Apellido1']);
	$apellido2 = utf8_encode($row['Apellido2']);
	$pago = $row['PAGO'];
}

echo "Nombre: " . $nombre . ";";
echo "<br>";
echo "Apellido1: " . $apellido1 . ";";
echo "<br>";
echo "Apellido2: " . $apellido2 . ";";
echo "<br>";
if ($pago==0)
	echo "Pago: NO;";
else
	echo "Pago: SI;";
echo "<br>";
echo "Alergias: ";

while($row = mysqli_fetch_assoc($resultado2)){
   echo "" . utf8_encode($row['Descripcion']) . "";
   echo " ";
}
echo ";";

echo "<br>";
echo "Alergias Diarias: ";

while($row = mysqli_fetch_assoc($resultado3)){
   echo "" . utf8_encode($row['Descripcion']) . "";
   echo " ";
}
?>
