def build(bld):
    bld.ap_program(
        program_name='AsatCopter',
        program_groups=['bin', 'core'], # Έτσι, το Waf ξέρει να τοποθετήσει το τελικό αρχείο στον φάκελο build/ΤΟ_BOARD_ΣΟΥ/bin/
        use='ap',
        source='AsatCopter.cpp',
    )

    # KATHE OXHMA THELEI ENA TETOIO BUILD FUNCTION GIA TO ./WAF