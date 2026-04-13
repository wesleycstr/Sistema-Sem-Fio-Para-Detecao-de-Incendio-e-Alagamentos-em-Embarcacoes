# Sistema-Sem-Fio-Para-Detecao-de-Incendio-e-Alagamentos-em-Embarcacoes
Trabalho de Conclusão de Curso Apresentado ao Curso Superior de Tecnologia em Redes de Computadores - IFRN 2026

## RESUMO
Neste projeto, descreve-se o desenvolvimento e a implementação de um sistema sem fio 
voltado ao monitoramento, detecção e emissão de alertas para incêndios e alagamentos em embarcações. 
O sistema foi concebido a partir da utilização de plataformas microcontroladas do tipo ESP8266, 
integradas a sensores de temperatura, umidade, gases e nível de água. A interface de monitoramento 
foi desenvolvida por meio do Grafana e a comunicação é realizada através de uma rede ad-hoc formada entre os sensores.
<img width="1280" height="720" alt="Imagens do TCC" src="https://github.com/user-attachments/assets/c670eca9-6129-4c8f-bb2c-273c1274db83" />

<details>
<summary>INTRODUÇÃO</summary>
  Detectar rapidamente incêndios e alagamentos em embarcações é fundamental para permitir
  uma resposta eficaz a esses acontecimentos, pois atrasos nessa identificação podem causar danos irreversíveis 
  aos equipamentos, à tripulação e ao meio ambiente. Incidentes como este, quando acontecem em ambientes marítimos, 
  costumam trazer grandes problemas devido ao fato de todos estarem isolados em alto mar e sem acesso a serviços 
  essenciais como por exemplo, socorro médico e bombeiros. Quando a fumaça ou a água se espalham pelos corredores, 
  fica difícil definir com precisão onde está o foco do problema, retardando assim o início da ação de combate. 
  Em instituições doutrinárias como a Marinha do Brasil, considera-se que o ideal é descobrir a ocorrência em até 
  três minutos após o seu início sob possibilidade de tornar-se algo fora de controle. Uma maneira de acelerar essa 
  detecção seria através de sistemas automatizados que monitoram e avisam sobre esses eventos. No entanto, tais sistemas 
  geralmente apresentam alto custo e complexidade de implementação quando levamos em consideração a ampla variedade dos 
  meios navais existentes na atualidade. Este artigo propõe uma alternativa de baixo custo e grande potencial baseada no 
  uso de dispositivos Internet of Things (IoT) em uma rede sem fio e Grafana. Considerando critérios como custo, facilidade 
  de instalação e funcionalidade, descreve-se como desenvolver um sistema utilizando plataformas microcontroladas 
  e diferentes sensores, capazes de monitorar e identificar avarias a bordo de embarcações de forma confiável.
</details>
<details>
<summary>METODOLOGIA</summary>
Esta seção descreve a estrutura do sistema proposto, bem como os componentes utilizados e a forma como ocorre a coleta, a transmissão e o processamento dos dados. O sistema foi idealizado com o objetivo de oferecer uma solução de baixo custo para monitoramento e alerta de incêndios e alagamentos em embarcações, priorizando simplicidade de implementação e eficiência na detecção.
  
## Visão geral do sistema
O sistema desenvolvido é composto por módulos de sensoriamento, comunicação e processamento, integrados de forma a permitir o monitoramento contínuo das condições ambientais da embarcação. Os sensores e microcontroladores são responsáveis pela coleta e envio de dados ao servidor por meio de protocolo HTTP. Também foi implementado uma rede ad-hoc entre os sensores de forma a manter a comunicação descentralizada e dinâmica. Dessa forma é possível estabelecer tráfego de dados por meio de rede sem fio, mesmo em embarcações construídas predominantemente com chapas de ferro, para se evitar o efeito de blindagem (Gaiola de Faraday) que bloqueia a passagem de ondas de rádio (RF) e dificulta a comunicação sem fio.

Os dados coletados são enviados a um servidor e armazenados em banco de dados, possibilitando análises históricas, identificação de padrões que possam indicar situações de risco. Esses dados armazenados também são utiliados para apresentação de dashboards por meio do Grafana.

Os usuários podem definir, através do Grafana, limiares de segurança para cada variável monitorada, de modo que, ao serem ultrapassados, mecanismos automáticos de alerta são acionados.

<img width="1582" height="899" alt="image" src="https://github.com/user-attachments/assets/bdfb6c97-015d-47d6-b0b3-e681c7fcd2d2" />

## Componentes de hardware
A arquitetura de hardware foi definida considerando disponibilidade, custo e compatibilidade entre os dispositivos. O sistema utiliza microcontroladores baseados no módulo ESP8266, escolhido devido à sua capacidade de processamento e conectividade Wi-Fi integrada. Este módulo possui 11 pinos de entrada/saída digital e 01 analógica, trabalha com com alimentação de 3,3V e suporta protocolos UART, I2C e SPI.

Para o monitoramento de gases, foi empregado um sensor do tipo MQ-7, adequado para detecção de monóxido de carbono (CO). Este sensor possui uma faixa de detecção de 10 a 10.000 ppm (partes por milhão), possui saída análogica e digital e trabalha com tensão de alimentação entre 3V e 5V DC.

A medição de temperatura é realizada por meio do sensor AHT10, que apresenta boa precisão e estabilidade para aplicações embarcadas. Este sensor possui uma faixa de medição de temperatura de -40°C a +80°C com uma precisão de ±0,3°C, trabalha com protocolo I2C e alimentação entre 1,8V e 3,6V.

Por fim, para monitoramento de alagamento foi utilizado o sensor de nível de água HW-028 que possui saída digital que indica presença ou ausência de água e trabalha com tensão de 3,3V a 5V. Também foi acrescentado um módulo relé com microcontrolador e conectividade Wi-Fi integrada para acionamento de dispositivos externos, como alarmes sonoros, permitindo uma resposta imediata em situações críticas. A tabela 01 demonstra o custo de aquisição aproximado para cada dispositivo.
<img width="864" height="376" alt="image" src="https://github.com/user-attachments/assets/b551c145-b13c-465f-8e89-730b33d56b99" />
</details>

