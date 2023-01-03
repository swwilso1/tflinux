grammar dhcpcd;

config_file
    : decl                          # declHelper1
    | config_file decl              # declHelper2
;

decl
    : comment                  # commentHelper
    | otherKeywordDecl         # otherHelper
    | optionDecl               # optionHelper
    | requireDecl              # requireHelper
    | slaacDecl                # slaacHelper
    | interfaceDecl            # interfaceHelper
    | controlGroupDecl         # controlHelper
;

comment
    : COMMENT                       # OneLineComment
    | comment COMMENT               # MultiLineComment
;

interfaceDecl
    : INTERFACE TEXT             # interfaceDetail
    | interfaceDecl staticLine   # staticHelper
;

staticLine
    : STATIC 'ip_address' '=' TEXT                   # ipv4Address
    | STATIC 'ip6_address' '=' TEXT                  # ipv6Address
    | STATIC 'routers' '=' server_list               # routers
    | STATIC 'domain_name_servers' '=' server_list   # dns
;

server_list
    : TEXT                          # simpleServer
    | server_list TEXT              # multipleServer
;

controlGroupDecl
    : CONTROLGROUP TEXT
;

optionDecl
    : OPTION optionValue (',' optionValue)*
;

optionValue
    : TEXT                   # optionText
    | 'domain_name_servers'  # optionDns
;

requireDecl
    : REQUIRE TEXT
;

slaacDecl
    : SLAAC 'hwaddr'          # slaacHwaddr
    | SLAAC 'private'         # slaacPrivate
;

otherKeywordDecl
    : HOSTNAME                 # hostname
    | CLIENTID                 # clientid
    | DUID                     # duid
    | PERSISTENT               # persistent
;

INTERFACE : 'interface' ;
STATIC : 'static';
CONTROLGROUP : 'controlgroup' ;
HOSTNAME : 'hostname' ;
CLIENTID : 'clientid' ;
DUID : 'duid' ;
PERSISTENT : 'persistent' ;
OPTION : 'option' ;
REQUIRE : 'require' ;
SLAAC : 'slaac';
TEXT : [a-z0-9:/._]+ ;
WHITESPACE : [\t\n\r ]+ -> skip;
COMMENT : '#' ~[\r\n]* '\r'? '\n';
DIGIT : [0-9] ;
ZERO_THROUGH_FIVE : [0-5] ;
ZERO_THROUGH_THREE : [0-3];
ZERO_THROUGH_TWO : [0-2];
