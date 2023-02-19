#include "customlistmodel.h"
#include <QDebug>

CustomListModel::CustomListModel(QObject *parent):
    QStringListModel(parent){
}

CustomListModel::CustomListModel(const QStringList &strings, QObject *parent):
    QStringListModel(strings, parent){

}

Qt::ItemFlags CustomListModel::flags (const QModelIndex & index) const {
    Qt::ItemFlags defaultFlags = QStringListModel::flags(index);
    if (index.isValid()){
        return (defaultFlags | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled) ^ Qt::ItemIsDropEnabled;
    }
    return defaultFlags;
}

QVariant CustomListModel::data(const QModelIndex &index,
                                 int role) const {
    if (!index.isValid())
        return QVariant();

    if(role == Qt::CheckStateRole)
        return checkedItems.contains(index) ?
                    Qt::Checked : Qt::Unchecked;

    else if(role == Qt::BackgroundColorRole)
        return checkedItems.contains(index) ?
                    QColor("#ffffb2") : QColor("#ffffff");

    return QStringListModel::data(index, role);
}

bool CustomListModel::setData(const QModelIndex &index,
                                const QVariant &value, int role){

    if(!index.isValid())
        return false;
    if(role != Qt::CheckStateRole)
    {
        return QStringListModel::setData(index, value, role);
    }
    if(value == Qt::Checked)
        checkedItems.insert(index);
    else
        checkedItems.remove(index);

    emit dataChanged(index, index);
    return true;
}

bool CustomListModel::setData(const QModelIndex &index, const QVariant &value)
{
    return QStringListModel::setData(index, value);
}

void CustomListModel::import(QString filePath)
{
    QFile inputFile( filePath );
    QTextStream in( &inputFile );
    in.setCodec("UTF-8");
    QStringList strList;
    QStringList paragraphs;
    QVector<int> checked;
    if( !inputFile.open( QIODevice::ReadOnly ) )    //text read
    {
        return ;
    }

    QString fullText;
    fullText.append(in.readAll());
    paragraphs = fullText.split('\n');
    for ( int index = 0; index <  paragraphs.length(); index++ )
    {
        strList += paragraphs[index].split(". ");
    }

    strList.removeAll(QString("")); // removing empty strings

    for( int index = 0; index <  strList.length(); index++  )
    {
        int dotIndex = strList[index].indexOf('.');      //removing tailing .
        if(dotIndex >= strList[index].length() - 2)
        {
            strList[index].remove(dotIndex, 1);
        }
        int rIndex = strList[index].indexOf('\r');      //removing \r
        if(rIndex >= strList[index].length() - 2)
        {
            strList[index].remove(rIndex, 1);
        }
        if(strList[index].indexOf("    ") == 0)
        {
            strList[index].remove(0, 4);
            checked.append(index);
        }
    }

    setStringList(strList);

    for( int index = 0; index <  checked.length(); index++  )
    {
        setData(this->index(checked[index]), Qt::Checked, Qt::CheckStateRole);
    }

    inputFile.close();
}

void CustomListModel::exportToFile(QString filePath)
{
    QFile outFile( filePath );
    QTextStream out( &outFile );
    if( !outFile.open( QIODevice::WriteOnly ) )
    {
        return ;
    }
    QString sentence;
    QString newLine = "";

    for( int index = 0; index <  this->rowCount(); index++  )
    {
        sentence = this->data(this->index(index, 0), Qt::DisplayRole).toString();

        if(index > 0)
            newLine = "\r\n";
        if( checkedItems.contains(this->index(index, 0)) )
        {
            out << newLine << "    " << sentence << ".";
        }
        else
        {
            out << " " << sentence << ".";
        }
    }
    outFile.close();
}

