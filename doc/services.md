
## HTTP Service
A HTTP Server hosts files in the following folder structure:
{name}/{instance}[/{major}][.{minor}][.{patch}]/{name}.xml

Examples:
ucip_mega/6000/ucip_mega.xml
ucip_mega/6000/3.12/ucip_mega.xml
ucip_mega/6000/3.13.4/ucip_mega.xml

When the application requests a configuration, the HTTP service uses the
specified version information to find the "best" matching configuration.

The default strategy works as follows (the examples use _ucip_mega 3.13.4_):
1. Check for the given version
   * ucip_mega/6000/3.13.4/ucip_mega.xml
2. Check for a matching major.minor version
   * ucip_mega/6000/3.13/ucip_mega.xml
3. Check for a matching major version
   * ucip_mega/6000/3/ucip_mega.xml
4. Check for a 'default' version
   * ucip_mega/6000/ucip_mega.xml


## FTP Service
See HTTP service.

## Local File Service
The local file service is a dummy and/or fallback solution to provide local
fallback configuration files, in case no other service was found or the
services could not provide a suitable configuration.

The local file service operates similar to the HTTP service, but instead of
querying and retrieving files from a web server, it uses the local file system.

## REST Service
The REST services sends a request containing the filter information to the
REST server. The server checks is cache/storage to find the best matching
configuration and returns the URL of the file / or the content of file. TBD
If using the URL, the content could be served from a different, static location.
