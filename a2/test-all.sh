#! /bin/bash

xx=$(tput setaf 1)$(tput bold)"XX"$(tput sgr0)
ok=$(tput setaf 2)$(tput bold)"OK"$(tput sgr0)
all_ok=true;

if ! make $1; then
  echo -e $xx" failed to build";
  exit;
fi

for input in ./samples/*.in; do
  output="./samples/$(basename $input .in).out"
  # if theres no matching output, just check the return is 0 and show ok
  if [ ! -f "$output" ]; then
    if $1 < $input > /dev/null; then
      echo -en $ok;
    else
      echo -en $xx;
      all_ok=false;
    fi
    echo ": sample '"$(basename $input)"'";
  else
    dc=$("$1" < $input | diff -U 0 $output - | grep ^@ | wc -l)
    if [ "$dc" -eq "0" ]; then
      echo -e $ok": sample '"$(basename $input)"'";
    else
      echo -e $xx": sample '"$(basename $input)"'";
      echo -e "\t$dc mismatched lines";
      all_ok=false;
    fi
  fi
done

if [ all_ok ]; then
  echo -e $(tput setaf 2)$(tput bold)"ALL OK"$(tput sgr0);
fi
