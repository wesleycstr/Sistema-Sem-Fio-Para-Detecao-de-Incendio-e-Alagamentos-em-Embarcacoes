<?php

function processarAlertas($conn){

    $sql = "

    SELECT

        sensores.id,

        sensores.temperatura_max,
        sensores.umidade_min,
        sensores.umidade_max,
        sensores.gas_max,

        dados.temperatura,
        dados.umidade,
        dados.gas_co

    FROM sensores

    LEFT JOIN dados
    ON dados.id = (

        SELECT id
        FROM dados d2
        WHERE d2.id_sensor = sensores.id
        ORDER BY data_hora DESC
        LIMIT 1

    )

    ";

    $result = $conn->query($sql);

    while($row = $result->fetch_assoc()){

        $status = 1;

        if(
            $row['temperatura'] > $row['temperatura_max']
        ){
            $status = 3;
        }

        if(
            $row['umidade'] < $row['umidade_min']
            ||
            $row['umidade'] > $row['umidade_max']
        ){
            $status = 2;
        }

        if(
            $row['gas_co'] > $row['gas_max']
        ){
            $status = 3;
        }

        $idSensor = $row['id'];

        $sqlUpdate = "

        INSERT INTO alarme
        (id_sensor, status)

        VALUES
        ('$idSensor', '$status')

        ON DUPLICATE KEY UPDATE
        status='$status'

        ";

        $conn->query($sqlUpdate);

    }

}
?>
