<?php

include 'conexao.php';

$id = $_GET['id'];

$sql = "

SELECT
    temperatura_max,
    umidade_min,
    umidade_max,
    gas_max
FROM sensores
WHERE id = '$id'

";

$result = $conn->query($sql);

$row = $result->fetch_assoc();

echo json_encode($row);

?>
