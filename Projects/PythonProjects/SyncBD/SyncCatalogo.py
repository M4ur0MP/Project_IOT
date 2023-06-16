import cx_Oracle
import pymongo
import json

try:
	con1 = cx_Oracle.connect('alumno/alumno@localhost:1521/xe')
    

except cx_Oracle.DatabaseError as er:
	print('Se produjo un error al intentar conectarse con oracle:', er)

else:
    try:
        client=pymongo.MongoClient('mongodb://127.0.0.1:27017/')

        mydb=client['Social']

        information = mydb.contenidos

        try:
            information.delete_many({}) #Elimino los registros en mongo
            cur = con1.cursor()
            cur2 = con1.cursor()
            cur3 = con1.cursor()
		# fetchall() is used to fetch all records from result set
            cur.execute('select codigo,titulo,descripcion,poster,duracion,fechaEstreno,clasificacion,director,elenco from contenido')
            while True:
                row = cur.fetchone()
                if row is None:
                    break
                cur2.execute('select b.nombre from contenidoGenero a inner join genero b on a.codigoGenero = b.codigo where a.codigoContenido='+ str(row[0]))       
                genero = [] 
                while True:
                    row2 = cur2.fetchone()
                    if row2 is None:
                        break
                    genero.append(row2[0])

                cur3.execute('select codigo, cantidadcapitulos from Temporada where codigoContenido ='+ str(row[0]))       
                temporada = [] 
                while True:
                    row3 = cur3.fetchone()
                    if row3 is None:
                        break
                   
                    temporada.append({"Temporada" : row3[0], "CantidadCapitulos" : row3[1] })
                    
                rec=[{
                    "titulo": row[1],
                    "descripcion" : row[2],
                    "poster" : row[3],
                    "duracion": row[4],
                    "fechaEstreno": row[5],
                    "clasificacion": row[6],
                    "director": row[7],
                    "elenco": row[8],
                    "genero": genero,
                    "temporada": temporada
                }]
                print(rec)               
                information.insert_many(rec)         
        except cx_Oracle.DatabaseError as er:
            print('Se produjo un error al obtener la información de la base:', er)

        except Exception as er:
            print('Error:'+str(er))

        finally:
            if cur:
                cur.close()
            if cur2:
                cur2.close()
            if cur3:
                cur3.close()

    except pymongo.errors as er:
	    print('Se produjo un error al intentar conectarse con mongoDb:', er)       
    finally:
        if client:
            client.close()
finally:
	if con1:
		con1.close()