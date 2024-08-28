#pragma once
#include <QObject>
#include <QList>

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

class MetaInfo;

enum ModelRoles {
	IdRole = Qt::UserRole + 1,
	RidRole,
	TypeRole,
	TitleRole,
	NickRole,
	AvatarRole,
	SnapshotRole,
	CategoryRole,
	FollowRole,
	FavRole,
	HeatRole,
	LiveRole,
	StartTimeRole,
	LastUpdateRole,
};

class MetaModel : public QAbstractListModel
{
	Q_OBJECT
public:
	MetaModel(QList<MetaInfo>& source, QObject *parent = 0);
	~MetaModel();

public:
	QHash<int, QByteArray> roleNames() const override;
	QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;

public:
	void changeAtIndex(int idx);
	void reset();
	void update(const MetaInfo& mi);
	using Updater = std::function<void(MetaInfo&)>;
	bool update(const QString& type, const QString& rid, Updater&& up);
	void remove(const QString& type, const QString& rid);

private:
	QList<MetaInfo>& source_;
};

class MetaModelProxy : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	MetaModelProxy(QObject* parent = 0);
	~MetaModelProxy();

public:
	void search(const QString& kw);

public:
	QHash<int, QByteArray> roleNames() const override;
	QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;

protected:
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
	bool filterAcceptsRow(int row, const QModelIndex& parent) const override;
};
