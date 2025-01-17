#!/bin/sh

GAME_INSTALL_RECEIPT="./build/install_manifest.txt"
GAME_PREFS=${HOME}/Library/Application\ Support/TTcards
GAME_LOGS_ROOT=${HOME}/Library/Logs/DiagnosticReports/
# GAME_LOGS_PATH="${GAME_LOGS_ROOT}/ttcards_*"
GAME_SAVED_APP_STATE=${HOME}/Library/Saved\ Application\ State/org.i8degrees.ttcards.savedState

if [[ -f ${GAME_INSTALL_RECEIPT} ]]; then
  echo "Remove game installation?"
  read -n1 GAME_INSTALL_RECEIPT_ANSWER

  if [[ ${GAME_INSTALL_RECEIPT_ANSWER} == "Y" || ${GAME_INSTALL_RECEIPT_ANSWER} == "y" ]]; then
    cat ${GAME_INSTALL_RECEIPT}
  else
    echo "Not removing ${GAME_INSTALL_RECEIPT}"
  fi
fi

if [[ -d "${GAME_PREFS}" ]]; then
  echo "Remove game preferences?"
  read -n1 GAME_PREFS_ANSWER

  if [[ ${GAME_PREFS_ANSWER} == "Y" || ${GAME_PREFS_ANSWER} == "y" ]]; then
    /bin/ls -lhas "${GAME_PREFS}"
  else
    echo "Not removing ${GAME_PREFS}"
  fi
fi

# if [[ -d "${GAME_LOGS_ROOT}" ]]; then
#   echo "Remove game logs?"
#   read -n1 GAME_LOGS_ANSWER

#   if [[ ${GAME_LOGS_ANSWER} == "Y" || ${GAME_LOGS_ANSWER} == "y" ]]; then
#     # /bin/ls -lhas "${GAME_LOGS_PATH}"
#     /bin/ls -h ${GAME_LOGS_ROOT} | grep "ttcards_*"
#   else
#     echo "Not removing ${GAME_LOGS_ROOT}"
#   fi
# fi

if [[ -d "${GAME_SAVED_APP_STATE}" ]]; then
  echo "Remove saved app state?"
  read -n1 GAME_SAVED_APP_STATE_ANSWER

  if [[ ${GAME_SAVED_APP_STATE_ANSWER} == "Y" || ${GAME_SAVED_APP_STATE_ANSWER} == "y" ]]; then
    /bin/ls -lhas "${GAME_SAVED_APP_STATE}"
  else
    echo "Not removing ${GAME_SAVED_APP_STATE}"
  fi
fi
