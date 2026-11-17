#include <QtWidgets>
class TaskModel : public QAbstractTableModel {
public:
    using QAbstractTableModel::QAbstractTableModel;
    int rowCount(const QModelIndex &parent={}) const override { return parent.isValid()?0:rows_.size(); }
    int columnCount(const QModelIndex &parent={}) const override { return parent.isValid()?0:2; }
    QVariant data(const QModelIndex &index,int role=Qt::DisplayRole) const override {
        if(!index.isValid() || index.row()<0 || index.row()>=rows_.size() || index.column()<0 || index.column()>1) return {};
        if(role==Qt::DisplayRole || role==Qt::EditRole) return rows_[index.row()][index.column()];
        return {};
    }
    QVariant headerData(int section,Qt::Orientation orientation,int role) const override {
        if(orientation==Qt::Horizontal && role==Qt::DisplayRole)
            return section==0?QStringLiteral("任务"):QStringLiteral("优先级");
        return QAbstractTableModel::headerData(section,orientation,role);
    }
    Qt::ItemFlags flags(const QModelIndex &index) const override {
        return index.isValid()?QAbstractTableModel::flags(index)|Qt::ItemIsEditable:Qt::NoItemFlags;
    }
    bool setData(const QModelIndex &index,const QVariant &value,int role) override {
        if(role!=Qt::EditRole || !index.isValid() || index.row()<0 || index.row()>=rows_.size() || index.column()<0 || index.column()>1) return false;
        const auto text=value.toString().trimmed(); if(text.isEmpty()) return false;
        rows_[index.row()][index.column()]=text; emit dataChanged(index,index,{Qt::DisplayRole,Qt::EditRole}); return true;
    }
private:
    QVector<QStringList> rows_{{QStringLiteral("学习布局"),"1"},{QStringLiteral("学习模型"),"2"}};
};
int main(int argc,char **argv) {
    QApplication app(argc,argv); QWidget w; auto *layout=new QVBoxLayout(&w);
    auto *search=new QLineEdit(&w); search->setPlaceholderText(QStringLiteral("按任务名过滤"));
    auto *table=new QTableView(&w); auto *model=new TaskModel(&w);
    auto *proxy=new QSortFilterProxyModel(&w); proxy->setSourceModel(model); proxy->setFilterKeyColumn(0);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive); table->setModel(proxy); table->setSortingEnabled(true);
    QObject::connect(search,&QLineEdit::textChanged,proxy,&QSortFilterProxyModel::setFilterFixedString);
    layout->addWidget(search); layout->addWidget(table); w.resize(600,350); w.show();
    if(app.arguments().contains("--smoke")) QTimer::singleShot(150,&app,&QCoreApplication::quit);
    return app.exec();
}
