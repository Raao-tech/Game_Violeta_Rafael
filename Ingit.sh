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
    echo -e "Se supone que soy un asistente, pero realmente estoy escalvizad@.... \n"
    echo -e "Llama a la policia please \n"
    sleep 1
    echo -e "\n-----------${YELOW}Opciones de administracion${RESET}------------\n";
    echo -e "3. ${GREEN}Guarda y manda mis cambios${RESET}\n";
    echo -e "4. Visualizar Ramas ${RED}(no funciona)${RESET}\n";
    echo -e "5. Estado del proyecto ${RED}(no funciona)${RESET}\n";
fi

echo -e "${adios}. ${RED}Adios Ingit ${RESET}\n";


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
        rm -rf *.git
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
    echo -e "\n${BLUE}--- Configuración Inicial del Repositorio ---${RESET}"
    echo -e "Parece que estamos empezando de cero (o desde un .zip)."
    echo -e "POR FAVOR LEE LO QUE TE ESTOY DICIENDO, NO CUESTA MUCHO"
    sleep 0.5
    
    # Verificación de seguridad
    read -p "¿Confirmas que quieres inicializar este directorio? (s/n): " respuesta
    if [[ ! "$respuesta" =~ ^(s|S|si|SI|y|Y|yes|YES)$ ]]; then
        echo -e "${RED}Operación cancelada.${RESET} Ingit se va a dormir un rato..."
        exit 0
    fi

    echo -e "\n${RED}¡PELIGRO!${RESET}"
    echo -e "Voy a sincronizar con la nube. Si tienes archivos aquí que no están en GitHub,"
    echo -e "${RED}SE PERDERÁN${RESET} para siempre (serán reemplazados por lo que hay en el repo)."
    read -p "¿Estás ABSOLUTAMENTE seguro? (escribe 'confirmar'): " resp_2

    if [[ "$resp_2" != "confirmar" ]]; then
        echo -e "Uff, de la que nos salvamos. Mejor asegúrate y vuelve luego.\n"
        exit 0
    fi

    clear
    echo -e "Iniciando la magia de ${GREEN}Ingit${RESET}...\n"

    # 1. Inicialización silenciosa
    git init -q
    git branch -m main

    # 2. Recolección de datos con validación simple
    while [[ -z "$username" ]]; do read -p "Introduce tu USERNAME de GitHub: " username; done
    while [[ -z "$email" ]]; do read -p "Introduce tu EMAIL de GitHub: " email; done
    while [[ -z "$token" ]]; do read -p "Introduce tu TOKEN (PAT): " token; done

    # 3. Configuración LOCAL (Solo afecta a esta carpeta)
    echo -e "\n${BLUE}[1/4]${RESET} Configurando identidad local..."
    git config user.name "$username"
    git config user.email "$email"
    # Evita que Git pregunte por el nombre de la rama al hacer pull
    git config pull.rebase false 

    # 4. Configuración del Remoto
    echo -e "${BLUE}[2/4]${RESET} Conectando con los servidores de la NASA (bueno, GitHub)..."
    # Limpiamos remotos previos para evitar el error "remote origin already exists"
    git remote remove origin 2>/dev/null
    
    # Usamos el repo que definiste al inicio del script
    url_con_token="https://${username}:${token}@${repo#https://}"
    git remote add origin "$url_con_token"

    # 5. Sincronización destructiva (Reset Hard)
    echo -e "${BLUE}[3/4]${RESET} Trayendo archivos de la nube..."
    git fetch origin main -q
    
    # Comprobar si el fetch funcionó antes de borrar todo
    if [ $? -eq 0 ]; then
        git reset --hard origin/main
        git branch --set-upstream-to=origin/main main
    else
        echo -e "${RED}Error:${RESET} No pude conectar. Revisa tu Token o el nombre de usuario."
        exit 1
    fi

    # 6. Actualización de Memoria Ingit
    echo -e "${BLUE}[4/4]${RESET} Actualizando mi memoria interna..."
    
    # Formateamos la fecha para el archivo
    fecha_actual=$(date +'%H\t%M\t%d\t%m\t%Y')
    
    # Actualizamos el archivo de stats
    sed -i "s/Aperturas.*/Aperturas\t1/" "$stats_file"
    sed -i "s/Ultimo_name.*/Ultimo_name\t$name/" "$stats_file"
    sed -i "s/Ultimo_user.*/Ultimo_user\t$username/" "$stats_file"
    sed -i "s/ultima_fecha.*/ultima_fecha\t$fecha_actual/" "$stats_file"

    # Creamos un commit de registro del login
    git add "$stats_file"
    git commit -m "Registro: $username inició sesión el $(date +'%d/%m/%Y')" > /dev/null

    echo -e "\n${GREEN}¡TODO LISTO, JEFE!${RESET}"
    echo -e "El repo está vinculado y limpio. Ya puedes empezar a romper cosas."
    echo -e "Prueba ejecutando: ${YELOW}ls -la${RESET} para ver tus archivos.\n"
elif [ "$opcion" == 3 ]; then
    read -p "$(echo -e "${YELOW}¿Qué cambios has hecho?${RESET} (Mensaje para el commit): ")" mensaje

    # 1. Guardar cambios locales del usuario
    echo -e "\nGuardando tus avances locales..."
    git add .
    git commit -m "$mensaje"

    # 2. PREPARAR EL REPO PARA LA NUBE (Resetear contador)
    # Guardamos tus datos actuales en variables temporales para no perderlos localmente
    temp_name=$name
    temp_user=$username
    
    echo -e "Preparando el archivo de memoria para que el repo aparezca como 'nuevo' en GitHub..."
    sed -i "s/Aperturas.*/Aperturas\t0/" "$stats_file"
    sed -i "s/Ultimo_name.*/Ultimo_name\tNAN/" "$stats_file"
    sed -i "s/Ultimo_user.*/Ultimo_user\tNAN/" "$stats_file"
    
    # Añadimos este cambio de "reset" al commit que vamos a subir
    git add "$stats_file"
    # Usamos --amend para que no aparezcan mil commits de "reset" en el historial
    GIT_EDITOR=true git commit --amend --no-edit

    # 3. TRAER CAMBIOS DE LA NUBE Y HACER REBASE
    echo -e "Sincronizando con la nube (Fetch & Rebase)..."
    git fetch origin main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

    # Intentamos el pull con rebase
    if ! git pull origin main --rebase; then
        echo -e "${RED}¡HOUSTON, TENEMOS UN CONFLICTO!${RESET}"
        echo -e "Alguien más ha modificado las mismas líneas que tú."
        echo -e "Abre los archivos, busca las marcas <<<<<< e intenta resolverlo."
        
        read -p "Presiona ${GREEN}ENTER${RESET} cuando hayas resuelto los conflictos..." listo
        
        git add .
        # Continuamos el rebase sin abrir editores de texto
        GIT_EDITOR=true git rebase --continue
    fi

    # 4. MANDAR INFORMACIÓN (Push)
    echo -e "Subiendo cambios limpios..."
    git push origin main --force-with-lease

    # 5. RESTAURAR LOCALMENTE (Para que tú sigas logueado)
    # Volvemos a poner tus datos en el archivo local para que Ingit te reconozca mañana
    sed -i "s/Aperturas.*/Aperturas\t1/" "$stats_file"
    sed -i "s/Ultimo_name.*/Ultimo_name\t$temp_name/" "$stats_file"
    sed -i "s/Ultimo_user.*/Ultimo_user\t$temp_user/" "$stats_file"
    
    echo -e "\n${GREEN}¡Todo listo, $temp_user!${RESET}"
    echo -e "${GREEN}registro de fecha actualizado${RESET}. Vuelve cuando quieras.\n"
    echo -e "Si necesitas algo mas, vuelve a llamarme\n"
    echo -e "Mientras tanto, me ire a sentar junto tu carpeta de matematicas de 20GB.... 
    ¿Sera que lo publico por telegram? mmmm bueno, luego vere. HASTA LUEGOO!!";
    
    sleep 1.5


fi