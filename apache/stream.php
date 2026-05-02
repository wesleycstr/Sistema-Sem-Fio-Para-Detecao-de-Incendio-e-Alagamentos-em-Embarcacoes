<?php

include 'conexao.php';

header('Content-Type: text/event-stream');
header('Cache-Control: no-cache');
header('Connection: keep-alive');

while (ob_get_level() > 0) {
    ob_end_flush();
}

ob_implicit_flush(true);

while(true){

$sql = "

SELECT 
    sensores.id,
    sensores.localizacao,
    alarme.status,
    dados.temperatura,
    dados.umidade,
    dados.gas_co,
    dados.data_hora

FROM sensores

LEFT JOIN alarme
ON sensores.id = alarme.id_sensor

LEFT JOIN dados
ON dados.id = (

    SELECT id
    FROM dados d2
    WHERE d2.id_sensor = sensores.id
    ORDER BY data_hora DESC
    LIMIT 1

)

ORDER BY sensores.id

";

$result = $conn->query($sql);

$html = "";

$erroGeral = false;

while($row = $result->fetch_assoc()){

    $classe = "";
    $cor = "#161616";
    $statusTexto = "DESCONHECIDO";

    $temperatura = $row['temperatura'] ?? '--';
    $umidade = $row['umidade'] ?? '--';
    $gas = $row['gas_co'] ?? '--';

    $ultimaAtualizacao = strtotime($row['data_hora']);
    $agora = time();

    /* diferença em segundos */
    $diferenca = $agora - $ultimaAtualizacao;

    /* sensor offline se passar de 10 segundos */
    $offline = $diferenca > 10;

    if($offline){

        $cor = "#424242";
        $statusTexto = "OFFLINE";
        $classe = "";
        $erroGeral = true;

    }else{

        switch($row['status']){

            case 1:
                $cor = "#2e7d32";
                $statusTexto = "NORMAL";
                break;

            case 2:
                $cor = "#f9a825";
                $statusTexto = "ATENÇÃO";
                break;

            case 3:
                $classe = "alarme";
                $statusTexto = "ALARME";
                break;
        }

    }

    $html .= "

    <tr class='$classe' style='background-color:$cor'>

        <td>{$row['id']}</td>

        <td>{$row['localizacao']}</td>

        <td>{$temperatura} °C</td>

        <td>{$umidade} %</td>

        <td>{$gas}</td>

        <td>{$statusTexto}</td>

        <td>{$row['data_hora']}</td>

    </tr>

    ";
}

/* mensagem de erro */

if($erroGeral){

    $html = "

    <tr>

        <td colspan='7'
            style='background:#b71c1c;
                   color:white;
                   text-align:center;
                   font-weight:bold;
                   font-size:18px;'>

            ⚠ ERRO: EXISTEM SENSORES SEM ATUALIZAÇÃO

        </td>

    </tr>

    " . $html;
}

echo "data: " . str_replace("\n", "", $html) . "\n\n";

flush();

sleep(1);

}
?>
