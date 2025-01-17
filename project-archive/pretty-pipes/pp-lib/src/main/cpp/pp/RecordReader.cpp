/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#include "RecordReader.h"

// Pretty Pipes
#include <pp/DataInputStream.h>

// Standard
#include <iostream>
#include <sstream>

#include "InputSplit.h"

using namespace std;

namespace pp
{

void RecordReader::setMapContext(HadoopPipes::MapContext& context)
{
  _context = &context;
  readInputSplit(context.getInputSplit());
}

void RecordReader::readInputSplit(const string& is)
{
  stringstream ss(is, stringstream::in);
  DataInputStream dis(ss);

  // read out the plugin list. This is only useful in the Java realm.
  dis.readUTF();
  // Read off the input split's class name. This is only useful in the Java realm.
  string className = dis.readUTF().toStdString();

  int dataSize = dis.readInt();
  string splitData;
  splitData.resize(dataSize);
  dis.read((char*)splitData.data(), dataSize);
  InputSplit* split = InputSplit::create(className);
  split->readFields(splitData);

  initialize(split, *_context);
}

}
