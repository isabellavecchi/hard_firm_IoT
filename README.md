# hard_firm_IoT
An WiFi connected Sound System with Alarm Clock, and real-time weather info

O projeto de firmware oficial é o wakeNdance. 

A tabela de partição da memoria, que substituí manualmente se encontra na pasta raiz de firmware.

Também foi necessário mudar a configuração de memória manualmente, mudei para 16Mbytes.

Este projeto foi inicializado na disciplina de "Hardware e Firmware - IoT" da UFU, e continuei o desenvolvimento para portifólio.

A placa utilizada foi a ESP32-S3 OTG.


  
  Features:
  
    - Wi-Fi com conexão controlada;
    
    - serviço web construído de forma a dar manutenção facilitada;
    
    - tasks dos botões contruída, mas aguardando a função de leitura do cartão ser concluída, para ter as funcionalidades costuradas;
    
    - freeRTOS, com controle de queue entre tasks;
    
    - servidor HTTP controlando o serviço web;
    
    - back-end e front-end funcionando;
    
    - socket NTP;
    
    - LED atualizando de acordo com o status da conexão;
    
    - display OLED 1306 funcionando, e com funções default, como o botão, aguardando a resolução de bugs do DMA;
    
    - features implementadas no modelo POO.

    
  
  To finish:
    - trocar a saída de som I2S por PDM, pelo fato de o módulo sendo utilizado, não ter tradução de I2S;
    
    - testar o DMA após a saída;
    
    - assim que as 2 camadas supracitadas estiverem funciando adequadamente, convertê-las para POO.
    
