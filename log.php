<?php
// populating $_GET
$DEBUG = true;
parse_str($_SERVER['QUERY_STRING'], $_GET);
$sid=$_GET['sid'];
$pm1 = $_GET['pm1'];
$pm25 = $_GET['pm25'];
$pm10 = $_GET['pm10'];
// do work

echo "Content-type: text/html\r\n";
echo "Connection: Close\r\n\r\n";
// start output here.
echo "<html><body>";

//$file = fopen("/home/anunez/www/cgi-bin/test.txt","a");
echo "id: ".$sid." pm25: ".$pm25."\n";
//echo fwrite($file,"id: ".$sid."pm25: ".$pm25."\n");
//fclose($file);
$servername = "localhost";
$username = "weblog";
$password = "pms3003";
$dbname = "weblog";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "INSERT INTO dust (sid, pm1, pm25, pm10) VALUES ('".$sid."', '".$pm1."', '".$pm25."', '".$pm10."')";


if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
echo "</body></html>";
?>
