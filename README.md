# kayito MuEmu 0.97k Full Package

## - Emulator: MuEmu

## - Main Base: 0.97k (0.97.11) KOR

## - Content:
- Client Original Version 0.97
- Encoder to generate the Main Info
- Full MuServer MuEmu 0.97
  - Connect Server
  - Join Server
  - Data Server
  - Game Server
- Tool to edit BMD/TXT files
- Tool to edit Accounts, Characters and Items
- Source Code of Everything above mentioned

## Updates

### Update 1
- Corregido problema con baul (era un error leve) -> [DataServer]
- Corregido problema con los stats y requisitos (ahora al repartir puntos por comando, actualiza en tiempo real) -> [GameServer y Main.dll]
- Corregido problema al minimizar (se bypasseó el pedazo de código que ocasionaba la desconexión al minimizar) -> [Main.dll]
### Update 2
- Corregido problema con baul al guardar items (era otro error leve de sincronización con el dataserver) -> [GameServer]

### Update 3
- Corregido problema al repartir todos los puntos, Points se mostraba en 0 y no quitaba los botones para sumar stats -> [Main.dll]
- Corregido problema al abrir Golden Archer no quitaba el minimapa -> [Main.dll]
- Corregido problema de items que se dropeaban con opciones que no correspondían (MuServer/Data/Item/ItemDrop.txt) -> [MuServer]

### Update 4
- Se redujo el tamaño del inventario al tamaño de la 97 original -> [Base de Datos][DataServer][GameServer]
- Se redujo el tamaño del baul al tamaño de la 97 original -> [Base de Datos][DataServer][GameServer]

### Update 5
- Se corrigió un error que no permitía juntar los kriss +0 -> [MuServer/Data/Quest/QuestObjective.txt]
- Se corrigió un error que bloqueaba el shop del NPC luego de hacer Reload Shop teniendo un Shop abierto -> [GameServer]

### Update 6
- Se añadió el sistema de Right Click Move hacia Warehouse, Trade y Chaos Mix -> [main.dll]
- Se corrigió un problema con el warehouse que te permitía extraer y superar el máximo de zen -> [GameServer.exe]
- Se añadió un switch para evitar que se puedan vender los items en los NPC al superar el máximo de zen -> [GameServer.exe][MuServer/GameServer/DATA/GameServerInfo-Common.dat]
- Se añadieron configuraciones para el costo de zen de cada mix en el chaos mix -> [GameServer.exe][MuServer/GameServer/DATA/GameServerInfo-ChaosMix.dat]
- Se añadió un sistema de sincronización para coordinar los rates del Chaos Mix con los del servidor -> [Main.dll][GameServer.exe][MuServer/GameServer/DATA/GameServerInfo-ChaosMix.dat]

### Update 7
- Se añadió el soporte al hardwareID para poder utilizarlo en MuServer/Data/Util/BlackList.dat -> [GameServer.exe]

### Update 8
- Se corrigió un error que al equipar o desequipar items, se visualizaban mal los stats -> [GameServer.exe]
- Se modificaron los mapas del cliente por los mapas originales junto con sus respectivos minimapas sincronizados al 100% -> [Cliente]
- Se añadió el Movelist funcional sincronizado con el move.txt del muserver -> [Main.dll][GameServer.exe]
- Se añadieron switchs para Minimap, Sky, Movelist y HealthBar -> [GetMainInfo.exe][Main.dll]
- Se corrigió un error que no previsualizaba correctamente al equiparse un Dinorant -> [GameServer.exe]
- Se corrigió un error que se veían unos cuadros negros del recorte del terreno al usar resoluciones superiores a 1280x1024 -> [Main.dll]
- Se corrigió y sincronizó todos los precios de compra de items por NPC en un 99% (Falta verificar las opciones excellent) -> [Main.dll][GameServer.exe]
- Se corrigió y sincronizó todos los precios de venta de items hacia un NPC en un 99% (Falta verificar las opciones excellent) -> [Main.dll][GameServer.exe]
- Se corrigió y sincronizó los costos de reparación de los items desde el inventario y desde el NPC -> [Main.dll][GameServer.exe]
- Se sincronizaron los stacks de items del main con el archivo ItemStack.txt del muserver -> [Main.dll][GameServer.exe]
- Se sincronizaron los precios de los items que se encuentren en ItemValue.txt en el muserver -> [Main.dll][GameServer.exe]
- Se corrigió el botón del zoom al clickear en el minimapa y ahora el personaje no camina al clickear ahí -> [Main.dll]

### Update 9
- Se corrigió un error en el ChaosMix que crasheaba el cliente -> [Cliente][GameServer.exe][main.dll]

### Update 10
- Se añadió un sistema de MapManager el cual permite manejar los mapas desde el GetMainInfo (Nombre, Movimiento, Acuático y Música) -> [Main.dll][GetMainInfo.exe]
- Se añadió un sistema de sincronización de los nombres de los subservers -> [Main.dll][ConnectServer.exe]

### Update 11:
- Corregido problema al resetear y quitar stats, no actualizaba los requisitos de los items -> [Main.dll]
- Corregido problema al dropear zen 0 -> [Main.dll][GameServer.exe]
- Corregido el problema del drop de zen 1 -> [GameServer.exe]
- Agregado caída de zen variable (antes dropeaba siempre una cantidad fija) -> [GameServer.exe]
- Agregado carpeta de sonido compatible (créditos: Kapocha33) -> [Cliente]
- Corregido problema de reconnect con sistema de desconexión de cuenta online -> [GameServer.exe]
- Corregido el problema del chat de guild y el guild master -> [GameServer.exe]
- Debido a la falta de auto attack, se optó por añadir un autoclick derecho con la tecla F9 -> [Main.dll]
- Añadido centrado de Server List en el Select Server (créditos: Kapocha33) -> [Main.dll]

### Update 12:
- Se corrigió el sistema de fruits [GameServer.exe][Main.dll]

### Update 13:
- Se corrigió un crasheo random al apuntar a un monster [Main.dll]
- Se corrigieron todos los sonidos del juego [Main.dll][Cliente/Data/Sound]

### Update 14:
- Se corrigió un crasheo que sucedía en un área específica de Stadium [Cliente][MuServer]
- Se corrigió los precios de compra, venta y reparación de items [Main.dll][GameServer.exe]
- Se añadió el sistema de juntar items con barra espaciadora [Main.dll]

### Update 15:
- Se añadió el sistema de Texturas para los items custom. Créditos: SetecSoft [Main.dll]
- Se añadió el sistema de Click Derecho para Equipar/Desequipar items. [Main.dll]
- Se añadió un sistema de empaquetamiento sincronizado con el ItemStack.txt del servidor que permite armar packs de cualquier item (ideal para jewels) [Main.dll][GameServer.exe][MuServer/Data/Item/ItemStack.txt]
- Se añadió un sistema de click derecho para desempaquetar de a uno los items empaquetados (ideal para jewels) [Main.dll]
- Se optimizó el sistema de protocolos del lado cliente para poder procesar y recibir todos los tipos de protocolos desde el servidor [Main.dll]
- Se optimizó el viewport de buffs y efectos del lado servidor, haciéndolo más rápido y eficiente [GameServer.exe]
- Se modificó la ubicación de los items custom, ahora permitiendo organizarlos y separarlos por carpetas individuales [Main.dll]
- Se corrigió un error que hacía que ciertos buffs y efectos desaparezcan al moverse de mapa o al desaparecer de la vista y reaparecer [Main.dll]
- Se corrigió un error que al intentar hacer Grand Reset, no se podía utilizar ningún otro comando y no se aplicaba el Grand Reset [DataServer.exe]
- Se corrigió un error que no se verificaba correctamente el nivel requerido para crear un MG [Main.dll][GameServer.exe]
- Se corrigió un error que calculaba mal el daño de la elfa cuando utilizaba Bows/Crossbows sin durabilidad [GameServer.exe]

### Update 16:
- Se separó por completo el sistema de Glow de los Custom Item. Créditos por la idea: Zeus [Main.dll][GetMainInfo.exe][MainInfo/CustomItem.txt][MainInfo/CustomGlow.txt]
- Se añadió al Custom Item la columna Skill para poder definir qué tipo de skill tendrán las armas [Main.dll][GetMainInfo.exe][MainInfo/CustomItem.txt]
- Se añadió el sistema de volumen para los sonidos del juego en el menu de opciones. Dicho sistema es compatible con el registro de Windows que todos los mains manejan [Main.dll]
- Se agregó el sonido al subir de nivel [Main.dll][Cliente/Data/Sound/pLevelUp.wav]
- Se corrigió que el minimapa no procese los clicks en su área correspondiente si la imagen del minimapa no existe en su correspondiente World en el cliente [Main.dll]
- Se corrigió un valor en el ItemValue que calculaba mal los precios de los items con muchas opciones [Main.dll][GameServer.exe]
- Se añadieron items custom a modo de ejemplo para que puedan continuar agregando por su cuenta sin problemas siguiendo dichas configuraciones

### Update 17:
- Se añadió el sistema de ItemOption para manipular las opciones de los items [Main.dll][GetMainInfo.exe][MainInfo/ItemOption.txt]
- Se sincronizó el sistema de manejo de opciones [Main.dll][GameServer.exe][MuServer/Data/Item/ItemOption.txt]
- Se sincronizó el sistema de ItemConvert (para requisitos, daños, durabilidad y opciones de los items) [Main.dll][GameServer.exe]
- Se sincronizó el sistema de ItemValue (para los precios de los items) [Main.dll][GameServer.exe]
- Se corrigieron los colores de los daños de todo tipo [GameServer.exe]
- Se corrigió poder ingresar a la misma cuenta usando mayúsculas y/o minúsculas [JoinServer.exe]
- Se corrigió utilizar el /move y soltar el item seleccionado para dupearlo visualmente [GameServer.exe]
- Se corrigió la animación del Power Slash del MG [Main.dll][Cliente/Data/Player/Player.bmd]
- Se corrigió que el Power Slash a veces no atacaba correctamente a los objetivos [GameServer.exe]
- Se modificó el campo ItemIndex en el BonusManager (ya no hace falta poner *,* y con un único * es suficiente) [GameServer.exe][MuServer/Data/Event/BonusManager.dat]

### Update 18:
- Se corrigió el drop de items con skill que no corresponden [GameServer.exe]
- Se corrigió el drop de items con nivel que no corresponden [MuServer/Data/Item/ItemDrop.txt]
- Se corrigió las opciones excellent de las alas cuando son full y cuando no (Damage y HP) [Main.dll][GameServer.exe]
- Se corrigió un crasheo inesperado que ocurría cuando se respawneaba luego de morir o de cambiar de mapa [Main.dll]
- Se corrigió un error visual por el cual el glow de los items no se mostraba correctamente acorde a su nivel [GameServer.exe]
- Se corrigió los requisitos del move respecto del MG (La ecuación para el MG es Requisito = ((MinLevel * 2) / 3)) [Main.dll]
- Se corrigió el respawn fuera del mapa de origen siendo menor a nivel 6 y estando fuera de safe zone [GameServer.exe]
- Se corrigieron errores en las tools que hacían que algunos txt sean mal interpretados [Tools/kayitoTools][Tools/kayitoEditor]
- Se sincronizó el editor de items con el ItemOption.txt para poder visualizar correctamente qué opciones puede llevar cada item [Tools/kayitoEditor]

### Update 19:
- Se migró el proyecto a GitHub para llevar mejor control de los cambios por update
- Se corrigió el problema que no permitía equipar los maces en la 2da mano [Main.dll]
- Se corrigió un error que generaba un crasheo en el main al intentar reparar un item muy caro [Main.dll]
- Se añadió un sistema de FONT en el que permite cambiar el tipo de fuente del cliente y el tamaño de letra [Main.dll]
- Se mejoró la interacción con el minimapa y el movelist respecto a los clicks [Main.dll]
- Se mejoró el dibujado de la barra de experiencia y el número que se muestra [Main.dll]
- Se reacomodaron algunos skills que funcionaban mal o no permitían atacarse entre usuarios (Por ej. Rageful Blow) [GameServer.exe]
- Se mejoró el sistema de cola de paquetes, reduciendo el consumo y aumentando la eficiencia (Créditos SetecSoft) [ConnectServer.exe][JoinServer.exe][DataServer.exe][GameServer.exe]
- Se corrigió el Weapon View en la zona safe. Ahora ambas armas se muestran como corresponde [Main.dll]
- Se implementó un nuevo MiniMapa llamado FullMap, que es generado por código automáticamente evitando así utilizar texturas (funcional para todos los mapas) [Main.dll]

### Update 20:
- Se corrigió un error en el MoveList que hacía que los colores de los nombres de los items dropeados se vean rojos [Main.dll]
- Se corrigió la posición del Skull Shield en la espalda [Main.dll]
- Se corrigió la interacción con click derecho en las entradas al Devil Square y Blood Castle en todos sus niveles [GameServer.exe]
- Se corrigió que al estar PK e intentar ingresar al Devil Square o al Blood Castle, no mostraba ningún mensaje [GameServer.exe]
- Se reconstruyeron los chequeos para equiparse items con click derecho [Main.dll][GameServer.exe]
- Se corrigió el MoveList que no se bloqueaba cuando el personaje es PK [Main.dll]
- Se optimizó el dibujado de la interface de los ejecutables. Ahora se recargan sólo cuando hay un log nuevo, reduciendo el consumo [ConnectServer][JoinServer][DataServer][GameServer]
- Se movieron las configuraciones de inicio que estaban en
"MuServer/GameServer/DATA/GameServerInfo - Common.dat"  
hacia otro archivo separado en  
"MuServer/GameServer/DATA/GameServerInfo - StartUp.dat"

### Update 21:
- Se corrigió el sistema de texturas que se continuaban perjudicando a medida que se agregaban más y más items (créditos: Zeus) [Main.dll]
- Se corrigieron los nombres en el HealthBar que se recortaban cuando el texto superaba el tamaño de la barra [Main.dll]
- Se añadió un sistema de reproductor musical para quitar definitivamente el MuPlayer.exe y que no haga falta integrar wzAudio.dll ni ogg.dll ni vorbisfile.dll [Main.dll]
- Se expandió el máximo de caracteres de los mensajes globales antes de que realice un salto de línea [Main.dll]
- Se migraron todas las lecturas de configuraciones al archivo Config.ini dentro del cliente (ya no se utiliza el registro de windows) [Main.dll]
- Se implementó un menu de opciones avanzadas que permite  
    - Cambiar el lenguaje (Eng, Spn, Por) sin salir del juego
    - Regular el volumen de los sonidos y la música por separado, y pausar/reproducir la música
    - Cambiar entre modo ventana y fullscreen y también cambiar la resolución del juego
    - Cambiar el tipo de fuente, el tamaño, la negrita y la cursiva
- Se corrigió que luego de reconectar, la barra de experiencia y el número mostraban datos erróneos
- Se corrigieron los textos de Bolts/Arrows que se muestran en la esquina superior derecha
- Se corrigieron los textos y las barras de HP de los pets que se muestran en la esquina superior derecha
- Se corrigieron los textos de los items que hacen falta reparar que se muestran en la esquina superior derecha