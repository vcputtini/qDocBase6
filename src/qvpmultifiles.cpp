/***************************************************************************
 * Copyright (c) 2007-2020                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase2 project, a images/documents           *
 * database program.                                                       *
 *                                                                         *
 * Redistribution and use in source and binary forms, with or without      *
 * modification, are permitted provided that the following conditions      *
 * are met:                                                                *
 * 1. Redistributions of source code must retain the above copyright       *
 *    notice, this list of conditions and the following disclaimer.        *
 * 2. Redistributions in binary form must reproduce the above copyright    *
 *    notice, this list of conditions and the following disclaimer in the  *
 *    documentation and/or other materials provided with the distribution. *
 * 4. Neither the name of the Author     nor the names of its contributors *
 *    may be used to endorse or promote products derived from this software*
 *    without specific prior written permission.                           *
 *                                                                         *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      *
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS  *
 * BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR   *
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF    *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS*
 * INTERRUPTION)                                                           *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,     *
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING   *
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE      *
 * POSSIBILITY OFSUCH DAMAGE.                                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "qvpmultifiles.h"

QvpMultiFiles::QvpMultiFiles()
{
}

QvpMultiFiles::~QvpMultiFiles()
{
    baData.clear();
}

/**
 * \brief Executa a leitura ou a escrita do arquivo de dados
 * Se for passado o nome de arquivo de leitura entao executa 
 * a funcao read()
 * Se for passado o nome do arquivo de saida entao executa a
 * funcao write()
 */
void QvpMultiFiles::execute()
{
   if(!fInput.isEmpty()) {
       int e = read();
       if(e != QFile::NoError)
           fileError(e);
   } 
   else if(!fOutput.isEmpty()) {
       int e = write(baData);
       if(e != QFile::NoError)
           fileError(e);
   }
}

/**
 * \brief Seta o nome do arquivo de entrada para a classe
 * @param fIn 
 */
void QvpMultiFiles::setInputFile(const QString fIn)
{
   fInput = fIn;
}

/**
 * \brief Seta o nome do arquivo de saida para a classe
 * @param fOut
 */
void QvpMultiFiles::setOutputFile(const QString fOut)
{  
   fOutput = fOut;
}


/**
 * \brief Executa a leitura do arquivo de entrada
 * Executa uma thread com QvpMultiFilesRead_p::read(QString)
 * e retorna o par QPair<int,QByteArray> 
 * QPair::first representa um inteiro com o QFile::error()
 * QPair::second representa o QByteArray com o conteudo do arquivo lido
 * Copia o resultado de QPair::second para o QByteArray privativo baData
 * @return int que representa o QFile::error()
 */
int QvpMultiFiles::read()
{
   pt2read = &QvpMultiFilesRead_p::read;

   QFuture< QPair<int,QByteArray> > fut0 = QtConcurrent::run(QvpMultiFilesRead_P, pt2read, fInput);

   if(fut0.result().second.size() > 0) {
      baData = fut0.result().second;
   }
   return(fut0.result().first);
}


/**
 * Executa uma thread com QvpMultiFiles::write(QByteArray, QString)
 * O primeiro parametro e o conteudo do arquivo a ser gravado
 * O segundo parametro e o nome do arquivo
 * @param baOut QByteArray contendo os dados que serao gravados no arquivo
 * @return  int que representa o QFile::error()
 */
int QvpMultiFiles::write(QByteArray baOut)
{
   pt2write = &QvpMultiFilesWrite_p::write;

   QFuture<int> fut0 = QtConcurrent::run(QvpMultiFilesWrite_P, pt2write, baOut, fOutput);
   return(fut0.result());
}


/**
 * \brief Retorna o tamanho do arquivo de entrada
 * @return quint64 - tamanho do arquivo
 */
quint64 QvpMultiFiles::sizeIn() const
{
   QFileInfo info(fInput);
   return(info.size());
}

/**
 * \brief Retorna o tamanho do arquivo de saida
 * @return quint64 - tamanho do arquivo
 */
quint64 QvpMultiFiles::sizeOut() const
{
   QFileInfo info(fOutput);
   return(info.size());
}

/**
 * \brief Verifica se o arquivo existe no disco ou nao
 * @return TRUE|FALSE
 */
bool QvpMultiFiles::exists(QString fname)
{
  QFileInfo info(fname);
  return(info.exists());
}

/**
 * \brief Seta dados via QByteArray para a classe usar
 * @param ba - QByteArray()
 */
void QvpMultiFiles::setData(QByteArray ba)
{
   baData.clear();
   baData = ba;
}


/**
 * \brief Retorna um QByteArray com os dados armazenados em baData
 * @return QByteArray::baData
 */
QByteArray QvpMultiFiles::data()
{
   return(baData);
}

/**
 * Esta funcao retorna o erro gerado pala funcao QFile::open().
 * Havendo um erro a funcao seta a mensagem de erro em portugues
 * via a emissao do sinal error()
 * e retorna FALSE.
 * @param fe int com o codigo de erro
 * @return TRUE|FALSE
 */
bool QvpMultiFiles::fileError(int fe)
{
    bool e=false;
    switch(fe) {
	case QFile::ReadError:
             emit error("Um erro ocorreu tentando ler o arquivo");
             break;
	case QFile::WriteError:
             emit error("Um erro ocorreu tentando gravar o arquivo");
             break;
	case QFile::FatalError:
             emit error("Um erro fatal ocorreu.");
             break;
	case QFile::ResourceError:
             emit error("Erro desconhecido. Talvez hajam muitos arquivos abertos.");
             break;
	case QFile::OpenError:
             emit error("O arquivo não pode ser aberto");
             break;
	case QFile::AbortError:
             emit error("A operação foi abortada.");
             break;
	case QFile::TimeOutError:
             emit error("O tempo se esgotou ao tentar processar esta operação.");
             break;
	case QFile::UnspecifiedError:
             emit error("Erro desconhecido.");
             break;
	case QFile::RemoveError:
             emit error("O arquivo não pode ser removido.");
             break;
	case QFile::RenameError:
             emit error("O arquivo não pode ser renomeado");
             break;
	case QFile::PositionError:
             emit error("A posição no arquivo não pode ser mudada.");
             break;
	case QFile::ResizeError:
             emit error("O Arquivo não pode ter seu tamanho alterado.");
             break;
	case QFile::PermissionsError:
             emit error("Erro nas permissões de acesso ao arquivo.");
             break;
	case QFile::CopyError:
             emit error("O arquivo não pode ser copiado.");
             break;
        default: { // sem erros
             emit error("");
             e = true;
        }
    }
    return(e);
}

//============================================================================

/**
 * \brief Le um arquivo do disco. Sera usada com ponteiro em uma thread
 * @param fname - path_completo+nome do arquivo
 *
 * Le o arquivo dado e coloca o conteudo em um bytearray
 * Retorna um par de dados como segue:
 * QPair<int, QByteArray>, sendo: <int> o codigo de erro QFile
 * <QByteArray> os dados lidos do arquivo no disco
 *
 * @return QPair<int,QByteArray>
 */
QPair<int,QByteArray> QvpMultiFilesRead_p::read(const QString fname)
{
   QPair<int, QByteArray> aux;
   QFile fileIn;
   fileIn.setFileName(fname);
   ba.clear();

   if(!fileIn.open(QIODevice::ReadOnly) ) {
       aux.first = fileIn.error();
       aux.second = ba;
   }
   else {
      while(!fileIn.atEnd() )
      {
         ba += fileIn.readLine();
      }
      aux.first = QFile::NoError;
      aux.second = ba;
      fileIn.close();
   }
   return(aux);
}


/**
 * \brief Escreve no disco os dados passados.  Sera usada com ponteiro em uma thread
 * @param baOut QByteArray contendo os dados a serem gravados no disco
 * @param fname Nome do arquivo
 * @return QFile::error()
 */
int QvpMultiFilesWrite_p::write(QByteArray baOut, const QString fname)
{
   QFile fileOut;
   fileOut.setFileName(fname);
   if(fileOut.open(QIODevice::WriteOnly)) {
       fileOut.write(baOut);
       fileOut.close();
   } 
   return(fileOut.error());
}
