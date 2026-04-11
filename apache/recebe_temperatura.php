<?php

$host = "mariadb";
$user = "root";
$pass = "senha";
$db   = "sensores";

$conn = new mysqli($host, $user, $pass, $db);

if ($conn->connect_error) {
    die("Erro de conexão: " . $conn->connect_error);
}

/* recebe dados via GET ou POST */
$sensor = $_REQUEST['sensor'] ?? null;
$temp   = $_REQUEST['temperatura'] ?? null;
$umid   = $_REQUEST['umidade'] ?? null;
$gas   = $_REQUEST['gas'] ?? null;

/* verifica se os dados existem */
if ($sensor === null || $temp === null) {
    die("Erro: parâmetros sensor e temperatura são obrigatórios");
}

/* insere no banco */
$sql = "INSERT INTO temperatura (sensor, valor, umidade, gas)
        VALUES ('$sensor','$temp', '$umid', '$gas')";

if ($conn->query($sql) === TRUE) {
    echo "OK";
} else {
    echo "Erro SQL: " . $conn->error;
}

$conn->close();

?>
