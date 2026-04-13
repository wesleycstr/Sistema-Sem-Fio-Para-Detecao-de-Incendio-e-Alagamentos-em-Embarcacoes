# Sistema-Sem-Fio-Para-Detecao-de-Incendio-e-Alagamentos-em-Embarcacoes
Trabalho de Conclusão de Curso Apresentado ao Curso Superior de Tecnologia em Redes de Computadores - IFRN 2026

## RESUMO
Neste Trabalho de Conclusão de Curso, descreve-se o desenvolvimento e a implementação de um sistema sem fio 
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

