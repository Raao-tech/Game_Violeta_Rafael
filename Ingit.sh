#!/bin/bash
repo="https://github.com/Raao-tech/Game_Violeta_Rafael.git";
adios="6";

#COLORSITOSSSSSSS
RED="\033[0;31m"
GREEN="\033[0;32m"
YELOW="\033[0;33m"
BLUE="\033[0;34m"
RESET="\033[0m"

# Revisamos la memoria de ingit
stats_file="otros/memoria_ingit.txt"
mkdir -p otros  # Crea la carpeta si no existe

if [ ! -f "$stats_file" ]; then
    echo -e "Aperturas\t0\nUltimo_name\tNAN\nUltimo_user\tNAN\nultima_fecha\t$(date +'%H\t%M\t%d\t%m\t%Y')" > "$stats_file"
fi

# Leer datos con un valor por defecto si falla awk
aperturas=$(awk '/Aperturas/ {print $2}' "$stats_file")
aperturas=${aperturas:-0} # Si está vacío, pon 0

ultimo_name=$(awk '/Ultimo_name/ {print $2}' "$stats_file")
ultimo_name=${ultimo_name:-"NAN"}

ultimo_user=$(awk '/Ultimo_user/ {print $2}' "$stats_file")
ultimo_user=${ultimo_user:-"NAN"}

if [ "$aperturas" -eq 0 ]; then
    echo -e "Hola! Soy tu asistente de Pre_prog, llamame ${RED} Ingit.${RESET}\n"
    read -p "Veo que es tu primera vez por aquí. ¿Quién eres? responde: " name
    read -p "Vale, ahora necesito conocer tu ${RED} USERNAME de GITHUB ${RESET} porque sin eso no se para que estas aqui: " username
    # Actualizamos el archivo: Aperturas pasa a 1 y guardamos el nombre
    sed -i "s/Aperturas.*/Aperturas\t1/" "$stats_file"
    sed -i "s/Ultimo_name.*/Ultimo_name\t$name/" "$stats_file"
    sed -i "s/Ultimo_user.*/Ultimo_user\t$username/" "$stats_file"

    sleep 1
    echo -e "¿En que te puedo ayudar hoy?\n"
    sleep 1
    echo -e "1. iniciar mi repo desde una compu de la uni\n";
    echo -e "2. iniciar mi repo desde mi compu ${RED}(no funciona, usala de arriba aunque estes en tu compu)${RESET}";
else
    name=$ultimo_name
    echo -e "¡Hola de nuevo, ${RED}${name}${RESET}! Soy ${GREEN}Ingit${RESET}.\n"
    sleep 1
    echo -e "¿En que te puedo ayudar esta vez? \n"
    sleep 1
    echo -e "\n-----------${YELOW}Opciones de administracion${RESET}------------\n";
    echo -e "3. Guarda y manda mis cambios\n";
    echo -e "4. Visualizar Ramas (no funciona)\n";
    echo -e "5. Estado del proyecto (no funciona)\n";
fi

echo -e "${adios}. Adios Ingit (No usar)\n";


play=1;
sleep 1
while [ $play -eq 1 ]; do
    read -p "Hare la opcion: " opcion;
    if [ "$opcion" == "${adios}" ]; then
        echo -e "Marchando ${name}!!"
        sleep 1
        echo -e "Dame un chance que estoy limpiando toda nuestra conversacion"
        sleep 2.5
        clear
        sleep 1
        echo -e "Todo listo mi brother"
        sleep 1
        echo -e "Por cierto, nadie debe de enterarse de esta conversacion"
        sleep 0.5 
        echo -e "¿Capichi mi pana?"
        sleep 1.5
        echo -e "... Mas te vale"
        sleep 3
        clear
        echo -e "Dicho eso, vente cuando quieras!!"
        sleep 2
        clear
        play=0;

        # Guardar fecha actual y resetear aperturas a 0
        fecha_actual=$(date +'%H\t%M\t%d\t%m\t%Y')
        sed -i "s/Aperturas.*/Aperturas\t0/" "$stats_file"
        sed -i "s/ultima_fecha.*/ultima_fecha\t$fecha_actual/" "$stats_file"

        #Limpiamos el config gnereal del repo (talk vez no sea lo mejor si es tu compu, pero por ahora lo dejaremos asi)
        echo -e "Por cierto, deberias asegurarte de volver a iniciar tus crendeciales si queires trabajar en otro repo, o usame para tus proyectos personales\n"
        echo -e "Soy una gran asistente jejeje... por favor, aca no me pagan, me estan explotando\n"
        echo -e "Bueno, continaure borrando cosas, ya estas avisado... llamame al +58 555 555 55 para pdoer ayudarte luego jejeje\n"
        sleep 3
        git config --global --unset user.name
        git config --global --unset user.email
        git remote remove origin

        GLOBIGNORE="Ingit.sh:Makefile"
        rm -rf *
        unset GLOBIGNORE
        exit 0;
    elif (( opcion < 1 || opcion >= adios )); then
        echo -e "${name} Has introducido una opcion invalida.... Dios, lo que me toca vivir\n";
    else 
        echo -e "Ok ${name}! En camino...\n";
        play=0;
    fi
done

sleep 1.5
clear
if [ "$opcion" == 1 ]; then

    echo -e "\nAntes de empezar, voy a crear un repo, se supone que has descargado esto desde un .zip\n"
    sleep 0.5
    
    read -p "¿Correcto? (responde  si, yes, s, y, todo lo demas sera un NO): " respuesta;
    
    sleep 0.1
    echo -e "Ok, otra cosa, Puede que todos los cambios que has hecho antes de iniciar la conexion con la nube";
    echo -e "SE PIERDANN, osea, TODOOOO, y se impondra lo que haya en la nube, asi que:\n";
    sleep 0.5
    
    read -p "¿Estas seguro de continuar? (responde  si, yes, s, y, todo lo demas sera un NO):" resp_2
    
    

    if [[ ! "$respuesta" =~ ^(si|yes|s|y|S|correct)$  ||  ! "$resp_2" =~ ^(si|yes|s|y|S|correct)$ ]]; then
        sleep 0.3
        echo -e "Pues ¿que haces aqui? Deja de joder\n";
        sleep 1;
        exit 0
    fi
    clear;
    sleep 1
    

    echo -e "Esta bien, ${RED} ya estas advertido ${RESET}.  El que avisa no es traidor....
    Dicho eso, ${GREEN} empecemos con la magia git ${BLUE} jejeje ${RESET}\n";
    
    #Esto es para dejar por default en el sistema el main
    git config --global init.defaultBranch main
    
    echo -e "${GREEN} Iniciando repo en el ${YELOW} directorio actual ${RESET}"
    git init
    git branch -m main

    sleep 0.35
    read -p "cual es tu ${YELOW} USERNAME ${RESET} de github? : " username
    read -p "cual es tu ${YELOW} EMAIL ${RESET} de github? : " email
    read -p "Cual es tu ${YELOW} TOKEN ${RESET} de uni? : " token
    
    echo -e "Configurando credenciales temporales..."
    git config --global user.email "$email"
    git config --global user.name "$username"

    sleep 0.5
    echo -e "${BLUE} Estableciendo ${RESET} el enlace HTTPS al repo"
    
    url_con_token="https://${username}:${token}@github.com/Raao-tech/Game_Violeta_Rafael.git"
    
    git remote prune origin
    git tag -d origin/main

    echo -e "${BLUE} Obvia lo de arriba ${YELOW} ^^^^ ${RESET}\n"
    git remote remove origin 2>/dev/null
    git remote add origin "$url_con_token"

    sleep 0.5
    echo -e "${GREEN} Pidiendo ${RESET} datos al repo...\n"
    git fetch origin
    
    echo -e "${BLUE} Sincronizando ${RESET} con la nube ${RED} (borrando basura local) ${RESET} ...\n"
    git reset --hard origin/main
    
    # Esto conecta tu rama local 'main' con la de GitHub para siempre
    git branch --set-upstream-to=origin/main main

    # Guardar fecha actual y reiniciar las aperturas para el proximo colabroardor
    sed -i "s/Aperturas.*/Aperturas\t0/" "$stats_file"
    fecha_actual=$(date +'%H\t%M\t%d\t%m\t%Y')
    sed -i "s/Aperturas.*/Aperturas\t0/" "$stats_file"
    
    git add otros/memoria_ingit.txt
    git commit -m "Ingreso de ${username} el dia ${fecha_actual}"
    echo -e "¡Listo! El repo está vinculado y al día.\n"
    

    
    sleep 1.5
    echo -e "Para verificar el estado de su repo, ejecuta    ${YELOW} git status ${RESET}"
    #Nos aseguramos de que los datos del actual usuario esten en la memoria local de Ingit.
    sed -i "s/Aperturas.*/Aperturas\t1/" "$stats_file"
    sed -i "s/Ultimo_user.*/Ultimo_user\t$username/" "$stats_file"
    sed -i "s/Ultimo_name.*/Ultimo_name\t$name/" "$stats_file"
    
elif [ "$opcion" == 3 ]; then
    read -p "$(echo -e "${YELOW}¿Qué cambios has hecho?${RESET} (Mensaje para el ${GREEN}commit${RESET}): ")" respuesta

    echo -e "OK. Primero guardemos tus cambios en el local\n"
    git add .
    git commit -m "$respuesta es un comentario preambulo"
    echo -e "OK. Comenzaremos viendo si hay errores de compatibilidad...\n"
    
    

    # 1. Intentamos traer lo de la nube
    git fetch origin main

    #Esto ees para que no haya problemas con el estado de la nube en la memoeria de Ingit, leugo lo ponemos otra vez en 1
    sed -i "s/Aperturas.*/Aperturas\t0/" "$stats_file"
    # 2. Intentamos un pull. Si falla, hay conflictos.
    if ! git pull origin main --rebase; then
            git add . 
            git commit -m "Cambios ehchos en main (rama con cambios locales)   para pasar a origin/main (rama global) "
            git checkout origin/main
            


        echo -e "${RED} ¡HOUSTON, TENEMOS UN CONFLICTO! ${RESET} 
        ${YELOW} Alguien ${RESET} ha tocado las ${RED} mismas líneas que tú ${RESET}. 
        Abre los archivos marcados, busca las marcas ${YELOW}'<<<<<<'${RESET}, límpialas y guarda.
        Se que usar tu criterio es algo costoso para ti, pero no pasa nada, intentalo.
        o llama al  ${GREEN} +58 PENDEJ@ HAS TU TRABAJO!! ${RESET}"
        git rebase main
        
        sleep 2
        # Aquí el script se detiene para que el humano arregle el código
        read -p "Presiona ${GREEN} ENTER ${RESET} cuando hayas ${RED} resuelto los conflictos ${RESET} en el código..." listo
        
        git add .
        git rebase --continue
    fi

    # Vemos que fecha es para poder guardarla y mandarla al commit
    fecha_actual=$(date +'%H\t%M\t%d\t%m\t%Y')
    sed -i "s/Aperturas.*/Aperturas\t0/" "$stats_file"
    
    # 3. Una vez limpio, mandamos 
    git add .
    git commit -m "$cambios $fecha_actual"
    git push origin main

    
    sed -i "s/ultima_fecha.*/ultima_fecha\t$fecha_actual/" "$stats_file"
    sed -i "s/Aperturas.*/Aperturas\t1/" "$stats_file"
    echo -e "Vale. Ya estaría, ${GREEN}registro de fecha actualizado${RESET}. Vuelve cuando quieras.\n"
    echo -e "Vale. Ya estaria, si necesitas algo mas, vuelve a llamarme\n"
    echo -e "Mientras tanto, me ire a sentar junto tu carpeta de matematicas de 20GB.... 
    ¿Sera que lo publico por telegram? mmmm bueno, luego vere. HASTA LUEGOO!!";
    
    sleep 1.5
    
        
fi