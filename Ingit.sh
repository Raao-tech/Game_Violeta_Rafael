#!/bin/bash
repo="https://github.com/Raao-tech/Game_Violeta_Rafael.git";
adios="6";
echo -e "Hola! Soy tu asistente de Pre_prog, llamame Ingit.\n";

read -p "Quien eres? responde: " name;
sleep 0.29
echo -e "Bienvenido ${name}, que quieres hacer?\n";
sleep 1
echo -e "1. iniciar mi repo desde una compu de la uni\n";
echo -e "2. iniciar mi repo desde mi compu";
echo -e "-----------Opciones de administracion------------";
echo -e "3. Guarda y manda mis cambios\n";
echo -e "4. Visualizar Ramas\n";
echo -e "5. Estado del proyecto\n";

echo -e "${adios}. Adios Ingit\n";


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

        git config --global --unset user.name
        git config --global --unset user.email
        git remote remove origin

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
    echo -e "Antes de empezar, voy a crear un repo, se supone que has descargado esto desde un .zip"
    sleep 0.5
    read -p "¿Correcto? (responde  si, yes, s, y, todo lo demas sera un NO): " respuesta
    if [[ ! "$respuesta" =~ ^(si|yes|s|y|S|correct)$ ]]; then
        sleep 0.3
        echo -e "Pues ¿que haces aqui? Deja de joder"
        exit 0
    fi
    clear;
    sleep 1
    echo -e "Iniciando repo en el directorio actual"
    git init
    git config --global init.defaultBranch main
    git branch -m main

    sleep 0.35
    read -p "cual es tu USERNAME de github? : " username
    read -p "cual es tu EMAIL de github? : " email
    read -p "Cual es tu TOKEN de uni? : " token
    
    echo -e "Configurando credenciales temporales..."
    git config --global user.email "$email"
    git config --global user.name "$username"

    echo -e "Estableciendo el enlace HTTPS al repo"
    
    url_con_token="https://${username}:${token}@github.com/Raao-tech/Game_Violeta_Rafael.git"
    
    git remote remove origin 2>/dev/null
    git remote add origin "$url_con_token"

    echo -e "Pidiendo datos al repo...\n"
    git fetch origin
    
    echo -e "Sincronizando con la nube (borrando basura local)...\n"
    git reset --hard origin/main
    
    # Esto conecta tu rama local 'main' con la de GitHub para siempre
    git branch --set-upstream-to=origin/main main

    echo -e "¡Listo! El repo está vinculado y al día.\n"
    
    sleep 1.5
    echo -e "Estado actual del repo:"
    git status
    ls -la
elif [ "$opcion" == 3 ]; then
    read -p "Vale, ¿Que Cambios has hecho?\n" cambios;

    echo -e "OK. Comenzaremos viendo si hay errores de compatibilidad...\n"

    # 1. Intentamos traer lo de la nube
    git fetch origin main

    # 2. Intentamos un pull. Si falla, hay conflictos.
    if ! git pull origin main --rebase; then
        echo -e "¡HOUSTON, TENEMOS UN CONFLICTO! 
        Alguien ha tocado las mismas líneas que tú. 
        Abre los archivos marcados, busca las marcas '<<<<<<', límpialas y guarda."
        
        # Aquí el script se detiene para que el humano arregle el código
        read -p "Presiona ENTER cuando hayas resuelto los conflictos en el código..." listo
        
        git add .
        git rebase --continue
    fi

    # 3. Una vez limpio, mandamos
    read -p "¿Qué cambios has hecho? (Mensaje para el commit): " cambios
    git add .
    git commit -m "$cambios"
    git push origin main

    echo -e "Vale. Ya estaria, si necesitas algo mas, vuelve a llamarme\n"
    sleep 2
    clear
        
fi



