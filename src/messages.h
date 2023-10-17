/***************************************************************************
 * Copyright (c) 2007-2023                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com
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
#ifndef MESSAGESNS_MESSAGES_H
#define MESSAGESNS_MESSAGES_H

#include <QMultiHash>
#include <QObject>
#include <QString>

/*!
 * \namespace MessagesNS
 * \brief General messages used in the program
 */
namespace MessagesNS {

constexpr const char* M_INTERNAL_ERROR =
  QT_TR_NOOP("Funtion: %0 Line: %1\nError: %2");
constexpr const char* M_INTERR_0001 = QT_TR_NOOP("Invalid Index");
constexpr const char* M_INTERR_0002 = QT_TR_NOOP("%0: nullptr");
// -------------
constexpr const char* M_FILE_NOTDELETED =
  QT_TR_NOOP("Erro arquivo [%0] não foi apagado");
constexpr const char* M_FILE_ERROR_READING =
  QT_TR_NOOP("Houve um erro tentando ler:<br>");
constexpr const char* M_FILE_ERROR_READ =
  QT_TR_NOOP("Um erro ocorreu tentando ler o arquivo");
constexpr const char* M_FILE_ERROR_WRITE =
  QT_TR_NOOP("Um erro ocorreu tentando gravar o arquivo");
constexpr const char* M_FILE_ERROR_FATAL = QT_TR_NOOP("Um erro fatal ocorreu");
constexpr const char* M_FILE_ERROR_RESOURCE =
  QT_TR_NOOP("Erro desconhecido. Talvez hajam muitos arquivos abertos");
constexpr const char* M_FILE_ERROR_OPEN =
  QT_TR_NOOP("O arquivo não pode ser aberto");
constexpr const char* M_FILE_ERROR_ABORT =
  QT_TR_NOOP("A operação foi abortada");
constexpr const char* M_FILE_ERROR_TIMEOUT =
  QT_TR_NOOP("O tempo se esgotou ao tentar processar esta operação");
constexpr const char* M_FILE_ERROR_UNSPECIFIED =
  QT_TR_NOOP("Erro desconhecido");
constexpr const char* M_FILE_ERROR_REMOVE =
  QT_TR_NOOP("O arquivo não pode ser removido");
constexpr const char* M_FILE_ERROR_RENAME =
  QT_TR_NOOP("O arquivo não pode ser renomeado");
constexpr const char* M_FILE_ERROR_POSITION =
  QT_TR_NOOP("A posição no arquivo não pode ser mudada");
constexpr const char* M_FILE_ERROR_RESIZE =
  QT_TR_NOOP("O Arquivo não pode ter seu tamanho alterado");
constexpr const char* M_FILE_ERROR_PERMISSIONS =
  QT_TR_NOOP("Erro nas permissões de acesso ao arquivo");
constexpr const char* M_FILE_ERROR_COPY =
  QT_TR_NOOP("O arquivo não pode ser copiado");
// -------------
constexpr const char* M_DB_RECSAVED =
  QT_TR_NOOP("Registro gravado com sucesso");
constexpr const char* M_DB_RECNOTSAVED = QT_TR_NOOP("Registro não foi gravado");
constexpr const char* M_DB_RECNOTFOUD = QT_TR_NOOP("Registro não encontrado");
constexpr const char* M_DB_RECDELETED =
  QT_TR_NOOP("Registro apagado com sucesso");
constexpr const char* M_DB_IMAGENOSAVED =
  QT_TR_NOOP("Imagem não foi gravada. A transação será revertida");
constexpr const char* M_DB_RECALREADYEXISTS = QT_TR_NOOP("[%0] já existe!");
// -------------
constexpr const char* GEN_WARN_00001 = QT_TR_NOOP(
  "Número do Endereço: Deve ser preenchido na faixa entre 0 até 999999");
constexpr const char* GEN_WARN_00002 = QT_TR_NOOP(
  "Não é possível informar os campos BR CNPJ e BR CPF ao mesmo tempo.");
constexpr const char* GEN_WARN_00003 = QT_TR_NOOP(
  "Código do Projeto: Deve ser preenchido na faixa entre 1 até 32767");
constexpr const char* GEN_WARN_00004 = QT_TR_NOOP(
  "Ano do Projeto: Deve ser igual ao ano corrente ou o ano corrente anterior");
constexpr const char* GEN_WARN_00005 = QT_TR_NOOP(
  "Campo %0: Nome do campo não foi informado. Mas o conteúdo foi preenchido");
constexpr const char* GEN_WARN_00006 =
  QT_TR_NOOP("Campo [%0]: não pode ser vazio");
constexpr const char* GEN_WARN_00007 =
  QT_TR_NOOP("Campo [%0]: contém caractéres inválidos\n\n!@#$%&*()+={}[]|");
constexpr const char* GEN_WARN_00008 =
  QT_TR_NOOP("%0: Inválido. DV não confere.<br>"
             "Informado: %1<br>"
             "Calculado: %2<br>"
             "Digitando o DV calculado o campo irá ser validado, porém isso "
             "não garante sua autenticidade.");
constexpr const char* GEN_WARN_00009 = QT_TR_NOOP("not used");
constexpr const char* GEN_WARN_00010 =
  QT_TR_NOOP("Se você confirmar que deseja apagar esta entidade, "
             "todos os documentos relacionados serão "
             "também removidos do banco de dados!");
constexpr const char* GEN_WARN_00011 =
  QT_TR_NOOP("Caractér Inválido: [\\] <br><br>Exemplo: imagens/*");
constexpr const char* GEN_WARN_00012 =
  QT_TR_NOOP("Para sua segurança o DocBase não permite a "
             "execução direta de programas.\n\n");
constexpr const char* GEN_WARN_00013 =
  QT_TR_NOOP("Não foi possível abrir o arquivo. Talvez não haja um aplicativo "
             "capaz de abri-lo instalado no seu computador.\n\n");
constexpr const char* GEN_WARN_00014 =
  QT_TR_NOOP("Ano inicial deve ser menor que o ano final");
constexpr const char* GEN_WARN_00015 =
  QT_TR_NOOP("Senha e/ou nome do operador são inválidos."
             "Tente novamente.");
constexpr const char* GEN_WARN_00016 =
  QT_TR_NOOP("Erro Interno: listViewImages. Comunique o desenvolvendor");
constexpr const char* GEN_WARN_00017 =
  QT_TR_NOOP("O arquivo [%0] não possui um formato válido "
             "para visualizar/imprimir.");
constexpr const char* GEN_WARN_00018 =
  QT_TR_NOOP("Selecione ao menos um documento");
constexpr const char* GEN_WARN_00019 =
  QT_TR_NOOP("Esta consulta irá gerar um relatório com [%0] páginas");
constexpr const char* GEN_WARN_00020 =
  QT_TR_NOOP("É aconselhável cancelar e gerar novamente em partes menores");
constexpr const char* GEN_WARN_00021 =
  QT_TR_NOOP("<br>Não foi possível obter os dados");
constexpr const char* GEN_WARN_00022 =
  QT_TR_NOOP("Um dos campos: CNPJ ou CPF devem ser informados");
constexpr const char* GEN_WARN_00023 = QT_TR_NOOP(
  "Mais que um documento deve ser selectionado para a importação em lotes.");

/*!
 * \brief The Tokens enum
 */
enum class Tokens
{
  INTERNAL_ERROR,
  FILE_ERROR_READING,
  FILE_NOTDELETED,
  FILE_ERROR_READ,
  FILE_ERROR_WRITE,
  FILE_ERROR_FATAL,
  FILE_ERROR_RESOURCE,
  FILE_ERROR_OPEN,
  FILE_ERROR_ABORT,
  FILE_ERROR_TIMEOUT,
  FILE_ERROR_UNSPECIFIED,
  FILE_ERROR_REMOVE,
  FILE_ERROR_RENAME,
  FILE_ERROR_POSITION,
  FILE_ERROR_RESIZE,
  FILE_ERROR_PERMISSIONS,
  FILE_ERROR_COPY,
  DB_WARN_RECSAVED,
  DB_WARN_RECNOTFOUND,
  DB_WARN_RECNOTSAVED,
  DB_WARN_RECDELETED,
  DB_WARN_IMAGENOTSAVED,
  DB_WARN_RECALREADYEXISTS,
  GEN_WARN_00001,
  GEN_WARN_00002,
  GEN_WARN_00003,
  GEN_WARN_00004,
  GEN_WARN_00005,
  GEN_WARN_00006,
  GEN_WARN_00007,
  GEN_WARN_00008,
  GEN_WARN_00009,
  GEN_WARN_00010,
  GEN_WARN_00011,
  GEN_WARN_00012,
  GEN_WARN_00013,
  GEN_WARN_00014,
  GEN_WARN_00015,
  GEN_WARN_00016,
  GEN_WARN_00017,
  GEN_WARN_00018,
  GEN_WARN_00019,
  GEN_WARN_00020,
  GEN_WARN_00021,
  GEN_WARN_00022,
  GEN_WARN_00023,
  INTERR_0001,
  INTERR_0002,
  NONE
};

/*!
 * \brief The CATEGORY enum
 */
enum class CATEGORY
{
  GENERAL = 0x00,
  DBASE = 0x01,
  NONE = 0xff
};

/*!
 * \class
 * \brief The Messages class
 * \note Singleton
 */
class Messages
{
public:
  static Messages& instance();
  Messages(const Messages&) = delete;
  Messages& operator=(const Messages&) = delete;

  Messages& set(Tokens&& token_);
  const QString text() noexcept;

private:
  Messages();
  ~Messages();

  CATEGORY categ_;
  QString msg_;
  QMultiHash<const Tokens, const QString> messages_hash_;

  inline void fillMap();
};

} // namespace MessagesNS

#endif // MESSAGESNS_MESSAGES_H
