import os.path as p
import runpy
import subprocess
import sys

# Συμβατότητα με Λειτουργικά Συστήματα (Linux LTS): Η έκδοση 3.10 είναι η προεπιλεγμένη έκδοση Python
# που έρχεται προεγκατεστημένη στις παλαιότερες εκδόσεις Ubuntu (όπως το Ubuntu 22.04 LTS) και Debian, 
# οι οποίες έχουν ακόμα ενεργή, επίσημη υποστήριξη (Standard Support). Η φιλοσοφία τους είναι: "Δεν αναγκάζουμε τον χρήστη
# να εγκαταστήσει περίεργες εκδόσεις Python. Υποστηρίζουμε τη μικρότερη έκδοση που έχει το πιο παλιό ενεργό Linux".

MIN_VER = (3, 10, 0)

if sys.version_info[:3] < MIN_VER:
    min_txt = ".".join(str(v) for v in MIN_VER)
    current_txt = ".".join(str(v) for v in sys.version_info[:3])
    print("ERROR: Python minimum supported version is",
          min_txt, "but you have", current_txt, file=sys.stderr)
    sys.exit(1)

def main():
    d = p.dirname(p.realpath(__file__))
    waf_light = p.join(d, 'external_modules', 'waf', 'waf-light')

    if not p.isfile(waf_light): # if it does not exist try to get it
      print('Missing waf submodule. Trying to get it from cloud')    

      try:
            subprocess.check_call(['git', 'submodule', 'update', '--init',
                                   'modules/waf'])
      except subprocess.CalledProcessError:
            print('Could not update submodule', file=sys.stderr)
            return 1
    
      print('Submodules OK, try running again')

      return 0
    runpy.run_path(waf_light, {}, "__main__")
    return 0  # το return το παιρνει το λειτουργικο για γνωση της καταστασης

if __name__ == "__main__": # αμα το κανω import αλλου το αρχειο αυτο δεν εκτελειται
    sys.exit(main())